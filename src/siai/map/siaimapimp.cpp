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

#include <algorithm>
#include <vector>

SIAIMapImp::SIAIMapImp() : m_commandStream{CommandStream::create()} {}
SIAIMapImp::~SIAIMapImp() = default;

void SIAIMapImp::setName(const std::string& name)
{
	m_name = name;
}

std::string SIAIMapImp::getName()
{
	return m_name;
}

void SIAIMapImp::executeCommand(const std::string& command)
{
    m_commandStream->executeAndLog(m_entities, command);

    diselectAll();
}

void SIAIMapImp::undo()
{
    m_commandStream->undo(m_entities);

    diselectAll();
}

void SIAIMapImp::redo()
{
    m_commandStream->redo(m_entities);

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

void SIAIMapImp::createDatabaseTables(DbConnector& connector)
{
	createCellsDbTable(connector);
	fillCellsDbTable(connector);

	createAgvsDbTable(connector);
}

void SIAIMapImp::createCellsDbTable(DbConnector& connector)
{
	std::string cellsTableName{SIAIGlobals::DB_CELLS_TABLE_PREFIX + m_name};

	SqlQueryData dataForCellsTable{cellsTableName, ICell::dbColumnNames, ICell::dbColumnTypes};
	SqlCreateTableQuery createCellsTableQuery(dataForCellsTable, ICell::primaryKeyName);

	tryToExecuteDbQuery(connector, createCellsTableQuery);
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

	tryToExecuteDbQuery(connector, cellsInsertQuery);
}

void SIAIMapImp::createAgvsDbTable(DbConnector& connector)
{
	std::string agvsTableName{SIAIGlobals::DB_AGVS_TABLE_PREFIX + m_name};

	SqlQueryData dataForAgvsTable{agvsTableName, IAgv::dbColumnNames, IAgv::dbColumnTypes};
	SqlCreateTableQuery createAgvsTableQuery(dataForAgvsTable, IAgv::primaryKeyName);

	tryToExecuteDbQuery(connector, createAgvsTableQuery);
}

void SIAIMapImp::tryToExecuteDbQuery(DbConnector& connector, const DbQuery& createQuery)
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

void SIAIMapImp::uploadChanges(DbConnector& connector)
{
	for(const auto& entity : m_entities)
	{
		entity->saveToDatabase(connector, SIAIGlobals::DB_AGVS_TABLE_PREFIX + m_name);
	}
}
