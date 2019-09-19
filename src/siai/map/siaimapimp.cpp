#include "map/siaimapimp.hpp"

#include "map/entities/mapentity.hpp"
#include "map/entities/cell.hpp"
#include "map/entities/agv.hpp"
#include "map/entities/rack.hpp"
#include "map/entities/item.hpp"

#include "map/cmd/cmdstream.hpp"

#include "map/exception.hpp"

#include "algorithms/pathfinder.hpp"

#include "util/reversion.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "globals.hpp"
#include "log.hpp"

#include <mysql++.h>

#include <algorithm>
#include <vector>

SIAIMapImp::SIAIMapImp(bool forConsole) : m_createdForConsole{forConsole}, m_commandStream{CommandStream::create()} {}
SIAIMapImp::~SIAIMapImp() = default;

void SIAIMapImp::setName(const std::string& name)
{
	m_name = name;

    updateDbTableNames();
}

std::string SIAIMapImp::getName()
{
	return m_name;
}

void SIAIMapImp::executeCommand(const std::string& command, DbConnector& connector)
{
	try
	{
		m_commandStream->executeAndLog(m_entities, connector, command);
	}
    catch(const std::exception& e)
    {
    	Log::warning(e.what());
    }

    diselectAll();
}

void SIAIMapImp::undo(DbConnector& connector)
{
    m_commandStream->undo(m_entities, connector);

    diselectAll();
}

void SIAIMapImp::redo(DbConnector& connector)
{
    m_commandStream->redo(m_entities, connector);

    diselectAll();
}

int SIAIMapImp::getNumberOfColumns() const noexcept
{
    return m_numberOfColumns;
}

int SIAIMapImp::getNumberOfRows() const noexcept
{
    return m_numberOfRows;
}

void SIAIMapImp::loadFromDb(DbConnector& connector)
{
    reset(0, 0);

    loadCellsFromDb(connector);
    loadAgvsFromDb(connector);
    loadRacksFromDb(connector);
}

void SIAIMapImp::updateFromDb(DbConnector& connector)
{
    updateAgvsFromDb(connector);
    updateRacksFromDb(connector);
}

void SIAIMapImp::reset(int numberOfColumns, int numberOfRows)
{
    m_numberOfColumns = numberOfColumns;
    m_numberOfRows = numberOfRows;

    m_selectedEntities.resize(0);
    m_entities.all.resize(0);
    m_entities.cells.resize(0);
    m_entities.agvs.resize(0);
    m_entities.racks.resize(0);
    
    m_commandStream->clear();

    Entities::generateMapCells(m_entities, m_numberOfColumns, m_numberOfRows);
}

void SIAIMapImp::repaint(Painter& painter)
{
    for(const auto& cell : m_entities.cells)
    {
        cell->draw(painter);
    }

    for(const auto& agv : m_entities.agvs)
    {
        agv->draw(painter);
    }

    for(const auto& rack : m_entities.racks)
    {
        rack->draw(painter);
    }
}

void SIAIMapImp::selectEntity(const PanelPoint& point)
{
    for(const auto& entity : Util::reverse(m_entities.all))
    {
        bool someEntityChanged = Entities::selectOrDiselectIfHasPointInside(*entity, point);

        if(someEntityChanged)
        {
            m_selectedEntities.push_back(entity);
            break;
        }
    }
}

void SIAIMapImp::diselectAll()
{
    for(const auto& entity : m_selectedEntities)
    {
        entity->diselect();
    }

    m_selectedEntities.resize(0);
}

Entities::AgvPtr SIAIMapImp::getAgvPtr(int id)
{
    return Entities::getAgvWithId(m_entities.agvs, id);
}

int SIAIMapImp::getSelectedEntityColumn() const noexcept
{
    MapPosition lastSelectedPosition{0, 0};

    for(const auto& entity : Util::reverse(m_selectedEntities))
    {
        if(entity->isSelected())
        {
            lastSelectedPosition = entity->getPosition();
        }
    }

    return lastSelectedPosition.column;
}

int SIAIMapImp::getSelectedEntityRow() const noexcept
{
    MapPosition lastSelectedPosition{0, 0};

    for(const auto& entity : Util::reverse(m_selectedEntities))
    {
        if(entity->isSelected())
            lastSelectedPosition = entity->getPosition();
    }

    return lastSelectedPosition.row;
}

int SIAIMapImp::getSelectedId() const noexcept
{
    int lastSelectedId{0};

    for(const auto& entity : m_selectedEntities)
    {
        if(entity->isSelected())
            lastSelectedId = entity->getId();
    }

    return lastSelectedId;
}

void SIAIMapImp::updateDbTableNames()
{
    m_cellsDbTableName = SIAIGlobals::DB_CELLS_TABLE_PREFIX + m_name;
    m_agvsDbTableName = SIAIGlobals::DB_AGVS_TABLE_PREFIX + m_name;
    m_racksDbTableName = SIAIGlobals::DB_RACKS_TABLE_PREFIX + m_name;
    m_itemsDbTableName = SIAIGlobals::DB_ITEMS_TABLE_PREFIX + m_name;
    m_ingressDbTableName = SIAIGlobals::DB_INGRESS_TABLE_PREFIX + m_name;
}

void SIAIMapImp::createDatabaseTables(DbConnector& connector)
{
	createCellsDbTable(connector);
	fillCellsDbTable(connector);

	createAgvsDbTable(connector);
    createRacksDbTable(connector);
    createItemsDbTable(connector);
    createIngressDbTable(connector);
}

void SIAIMapImp::uploadChanges(DbConnector& connector)
{
	for(const auto& entity : m_entities.all)
	{
		entity->updateInDatabase(connector, m_name);
	}
}

bool SIAIMapImp::moveAgvToCellWithId(DbConnector& connector, Entities::AgvPtr& agv, int idCell)
{
    Entities::CellPtr cell;
    cell = Entities::getCellWithId(m_entities.cells, idCell);

    if(!cell)
        return false;

    MapPosition cellPosition = cell->getPosition();

    agv->setPosition(cellPosition);
    if(hasLiftedRack(agv))
    {
        Entities::RackPtr liftedRack = Entities::getRackWithId(m_entities.racks, agv->getLiftedRackId());
        liftedRack->setPosition(cellPosition);

        agv->updateInDatabase(connector, m_name);
        liftedRack->updateInDatabase(connector, m_name);
        return true;
    }

    agv->updateInDatabase(connector, m_name);
    return true;
}

bool SIAIMapImp::assignNewTaskToAgv(DbConnector& connector, Entities::AgvPtr& agv)
{
    SqlQueryData dataToSelect{m_ingressDbTableName, {"id", "rackid"}};

    // We select only the first task with an "agvid" of value  "0" which means it is unassigned
    SqlSelectQuery selectCellsQuery(dataToSelect, std::string("WHERE (`agvid` = '0')  LIMIT 0, 1"));

    std::vector<DbRow> cellsRows;
    bool querySuccess = tryQueryAndStore(connector, selectCellsQuery, cellsRows);

    if(!querySuccess)
        return false;

    if(cellsRows.empty())
        return false;

    DbRow firstTask = cellsRows[0];

    int taskId = firstTask[0];
    int taskRackId = firstTask[1];

    IngressTask newTask(taskId, taskRackId);

    auto rack = Entities::getRackWithId(m_entities.racks, taskRackId);

    MapPosition agvPosition = agv->getPosition();
    MapPosition rackPosition = rack->getPosition();

    MapGrid tmpMapGrid = Entities::generateMapGrid(m_entities, agvPosition, rackPosition);
    PathFinder tmpPathFinder(m_numberOfColumns, m_numberOfRows);
    MapPath pathToRack = tmpPathFinder.find(tmpMapGrid, agvPosition, rackPosition);

    newTask.setPath(pathToRack);

    bool taskAssigned = agv->assignTask(connector, m_name, std::make_shared<IngressTask>(newTask));

    return taskAssigned;
}

bool SIAIMapImp::updateTaskForAgv(Entities::AgvPtr& agv)
{
    MapPosition ingressCellPosition{-1, -1};
    for(const auto& cell : m_entities.cells)
    {
        if(cell->getEntityName() == "Ingress")
        {
            ingressCellPosition = cell->getPosition();
            break;
        }
    }

    if(ingressCellPosition.column == -1 || ingressCellPosition.row == -1)
        return false;

    MapPosition agvPosition = agv->getPosition();
    MapGrid tmpMapGrid = Entities::generateMapGrid(m_entities, agvPosition, ingressCellPosition);
    PathFinder tmpPathFinder(m_numberOfColumns, m_numberOfRows);
    MapPath pathToIngressCell = tmpPathFinder.find(tmpMapGrid, agvPosition, ingressCellPosition);

    agv->updateTaskPath(pathToIngressCell);

    return true;
}

bool SIAIMapImp::liftRackInPosition(DbConnector& connector, const MapPosition& position)
{
    auto rack = Entities::getRackByPosition(m_entities.racks, position);

    if(rack)
    {
        rack->lift();
        rack->updateInDatabase(connector, m_name);
        return true;
    }

    return false;
}

bool SIAIMapImp::dropRackInPosition(DbConnector& connector, const MapPosition& position)
{
    auto rack = Entities::getRackByPosition(m_entities.racks, position);

    if(rack)
    {
        rack->drop();
        rack->updateInDatabase(connector, m_name);
        return true;
    }

    return false;
}

bool SIAIMapImp::hasLiftedRack(Entities::AgvPtr& agv)
{
    int rackId = agv->getLiftedRackId();
    if(rackId < IRack::MIN_ID)
        return false;

    auto rack = Entities::getRackWithId(m_entities.racks, rackId);
    if(rack)
    {
        if(rack->isLifted())
            return true;
    }
    
    return false;
}

void SIAIMapImp::enableDirectionMarkers(bool value)
{
    ICell::enableDirectionMarkers(value);
}

void  SIAIMapImp::loadCellsFromDb(DbConnector& connector)
{
    SqlQueryData dataToSelectFromCells{m_cellsDbTableName, ICell::dbColumnNames};

    SqlSelectQuery selectCellsQuery(dataToSelectFromCells);

    std::vector<DbRow> cellsRows;
    tryQueryAndStore(connector, selectCellsQuery, cellsRows);

    Entities::loadCellsFromQueryRows(m_entities, cellsRows);

    m_numberOfColumns = Entities::getNumberOfMapColsFromDbRows(cellsRows);
    m_numberOfRows = Entities::getNumberOfMapRowsFromDbRows(cellsRows);
}

void SIAIMapImp::loadAgvsFromDb(DbConnector& connector)
{
    SqlQueryData dataToSelectFromAgvs{m_agvsDbTableName, IAgv::dbColumnNames};

    SqlSelectQuery selectAgvsQuery(dataToSelectFromAgvs);

    std::vector<DbRow> agvsRows;
    tryQueryAndStore(connector, selectAgvsQuery, agvsRows);

    Entities::loadAgvsFromQueryRows(m_entities, agvsRows);
}

void SIAIMapImp::loadRacksFromDb(DbConnector& connector)
{
    SqlQueryData dataToSelectFromRacks{m_racksDbTableName, IRack::dbColumnNames};

    SqlSelectQuery selectRacksQuery(dataToSelectFromRacks);

    std::vector<DbRow> racksRows;
    tryQueryAndStore(connector, selectRacksQuery, racksRows);

    Entities::loadRacksFromQueryRows(m_entities, racksRows);
}

void SIAIMapImp::updateAgvsFromDb(DbConnector& connector)
{
    SqlQueryData dataToSelectFromAgvs{m_agvsDbTableName, IAgv::dbColumnNames};

    SqlSelectQuery selectAgvsQuery(dataToSelectFromAgvs);

    std::vector<DbRow> agvsRows;
    tryQueryAndStore(connector, selectAgvsQuery, agvsRows);

    Entities::updateAgvsFromQueryRows(m_entities.agvs, agvsRows);
}

void SIAIMapImp::updateRacksFromDb(DbConnector& connector)
{
    SqlQueryData dataToSelect{m_racksDbTableName, IRack::dbColumnNames};

    SqlSelectQuery selectQuery(dataToSelect);

    std::vector<DbRow> racksRows;
    tryQueryAndStore(connector, selectQuery, racksRows);

    Entities::updateRacksFromQueryRows(m_entities.racks, racksRows);
}

void SIAIMapImp::createCellsDbTable(DbConnector& connector)
{
	SqlQueryData dataForCellsTable{m_cellsDbTableName, ICell::dbColumnNames, ICell::dbColumnTypes};
	SqlCreateTableQuery createCellsTableQuery(dataForCellsTable, ICell::primaryKeyName);

	tryQueryWithoutResults(connector, createCellsTableQuery);
}

void SIAIMapImp::fillCellsDbTable(DbConnector& connector)
{
	std::vector<std::vector<std::string>> cellsValues;

	for(const auto& cell : m_entities.cells)
	{
		int direction = static_cast<int>(cell->getDirection());

		cellsValues.push_back(std::vector<std::string>{ std::to_string(cell->getId()),
			std::to_string(cell->getPosition().column), std::to_string(cell->getPosition().row),
			std::to_string(direction), cell->getEntityName()});
	}

	SqlMultipleQueryData cellsDataToInsert{SIAIGlobals::DB_CELLS_TABLE_PREFIX + m_name, ICell::dbColumnNames,
		cellsValues};
	SqlMultipleInsertQuery cellsInsertQuery(cellsDataToInsert);

	tryQueryWithoutResults(connector, cellsInsertQuery);
}

void SIAIMapImp::createAgvsDbTable(DbConnector& connector)
{
	SqlQueryData dataForAgvsTable{m_agvsDbTableName, IAgv::dbColumnNames, IAgv::dbColumnTypes};
	SqlCreateTableQuery createAgvsTableQuery(dataForAgvsTable, IAgv::primaryKeyName);

	tryQueryWithoutResults(connector, createAgvsTableQuery);
}

void SIAIMapImp::createRacksDbTable(DbConnector& connector)
{
	SqlQueryData dataForRacksTable{m_racksDbTableName, IRack::dbColumnNames, IRack::dbColumnTypes};
	SqlCreateTableQuery createRacksTableQuery(dataForRacksTable, IRack::primaryKeyName);

	tryQueryWithoutResults(connector, createRacksTableQuery);
}

void SIAIMapImp::createItemsDbTable(DbConnector& connector)
{
	SqlQueryData dataForTable{m_itemsDbTableName, AItem::dbColumnNames, AItem::dbColumnTypes};
	SqlCreateTableQuery createTableQuery(dataForTable, AItem::primaryKeyName);

	tryQueryWithoutResults(connector, createTableQuery);
}

void SIAIMapImp::createIngressDbTable(DbConnector& connector)
{
    // As of this version, the "Ingress" table has the exact same structure as the "Items" table.
    // Plus a column for the id of the AGV assigned to carry the task.
    // Plus a column to mark it as done
    std::vector<std::string> ingressDbColumnNames = AItem::dbColumnNames;
    ingressDbColumnNames.push_back(std::string("agvid"));
    ingressDbColumnNames.push_back(std::string("done"));

    std::vector<std::string> ingressDbColumnTypes = AItem::AItem::dbColumnTypes;
    ingressDbColumnTypes.push_back(std::string("INT NULL DEFAULT 0"));
    ingressDbColumnTypes.push_back(std::string("TINYINT NULL DEFAULT 0"));
    
    SqlQueryData dataForTable{m_ingressDbTableName, ingressDbColumnNames, ingressDbColumnTypes};
	SqlCreateTableQuery createTableQuery(dataForTable, AItem::primaryKeyName);

	tryQueryWithoutResults(connector, createTableQuery);
}

bool SIAIMapImp::tryQueryWithoutResults(DbConnector& connector, const DbQuery& createQuery)
{
	try
	{
		connector.executeQueryWithoutResults(createQuery);
	}
	catch(const std::exception& e)
	{
		Log::warning(std::string("Error al enviar comando a base de datos: ") + e.what(), m_createdForConsole);

		reset(0, 0);

        return false;
	}

    return true;
}

bool SIAIMapImp::tryQueryAndStore(DbConnector& connector, const DbQuery& query, std::vector<DbRow>& vector)
{
    try
	{
		connector.executeQueryAndStoreInVector(query, vector);
	}
	catch(const std::exception& e)
	{
		Log::warning(std::string("Error al enviar comando a base de datos: ") + e.what(), m_createdForConsole);

		reset(0, 0);

        return false;
	}

    return true;
}