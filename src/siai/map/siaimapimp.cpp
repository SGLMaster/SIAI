#include "map/siaimapimp.hpp"
#include "map/entities/mapentity.hpp"
#include "map/entities/cell.hpp"
#include "map/entities/agv.hpp"
#include "map/cmd/cmdstream.hpp"

#include "util/reversion.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "globals.hpp"
#include "log.hpp"

#include <mysql++.h>

#include <algorithm>
#include <vector>

SIAIMapImp::SIAIMapImp() : m_commandStream{CommandStream::create()} {}
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
}

void SIAIMapImp::reset(int numberOfColumns, int numberOfRows)
{
    m_numberOfColumns = numberOfColumns;
    m_numberOfRows = numberOfRows;

    m_selectedEntities.resize(0);
    m_entities.resize(0);

    Entities::generateMapCells(m_entities, m_numberOfColumns, m_numberOfRows);
}

void SIAIMapImp::repaint(Painter& painter)
{
    for(const auto& entity : m_entities)
    {
        entity->draw(painter);
    }
}

void SIAIMapImp::selectEntity(const PanelPoint& point)
{
    for(const auto& entity : Util::reverse(m_entities))
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
}

void SIAIMapImp::createDatabaseTables(DbConnector& connector)
{
	createCellsDbTable(connector);
	fillCellsDbTable(connector);

	createAgvsDbTable(connector);
}

void SIAIMapImp::uploadChanges(DbConnector& connector)
{
	for(const auto& entity : m_entities)
	{
		entity->saveToDatabase(connector, m_name);
	}
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

void SIAIMapImp::createCellsDbTable(DbConnector& connector)
{
	SqlQueryData dataForCellsTable{m_cellsDbTableName, ICell::dbColumnNames, ICell::dbColumnTypes};
	SqlCreateTableQuery createCellsTableQuery(dataForCellsTable, ICell::primaryKeyName);

	tryQueryWithoutResults(connector, createCellsTableQuery);
}

void SIAIMapImp::fillCellsDbTable(DbConnector& connector)
{
	std::vector<std::vector<std::string>> cellsValues;

	for(const auto& cell : m_entities)
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

void SIAIMapImp::tryQueryWithoutResults(DbConnector& connector, const DbQuery& createQuery)
{
	try
	{
		connector.executeQueryWithoutResults(createQuery);
	}
	catch(const std::exception& e)
	{
		Log::warning(std::string("Error al enviar comando a base de datos: ") + e.what());

		reset(0, 0);
	}
}

void SIAIMapImp::tryQueryAndStore(DbConnector& connector, const DbQuery& query, std::vector<DbRow>& vector)
{
    try
	{
		connector.executeQueryAndStoreInVector(query, vector);
	}
	catch(const std::exception& e)
	{
		Log::warning(std::string("Error al enviar comando a base de datos: ") + e.what());

		reset(0, 0);
	}
}