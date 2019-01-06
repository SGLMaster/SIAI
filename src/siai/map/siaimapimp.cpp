#include "map/siaimapimp.hpp"
#include "map/entities/mapentity.hpp"
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

void SIAIMapImp::createDatabaseTables(DbConnector& connector, const std::string& mapName)
{
	std::string cellsTableName{SIAIGlobals::DB_CELLS_TABLE_PREFIX + mapName};
	std::vector<std::string> cellsColsNames{ "id", "column", "row" };

	createCellsDbTable(connector, cellsTableName, cellsColsNames);
	fillCellsDbTable(connector, cellsTableName, cellsColsNames);
}

void SIAIMapImp::createCellsDbTable(DbConnector& connector, const std::string& tableName,
		std::vector<std::string> colsNames)
{
	SqlQueryData dataForCellsTable{tableName, colsNames, { "INT NOT NULL", "INT NOT NULL", "INT NOT NULL"}};
	std::string primaryKey = "id";

	SqlCreateTableQuery createCellsTableQuery(dataForCellsTable, primaryKey);

	tryToExecuteDbQuery(connector, createCellsTableQuery);
}

void SIAIMapImp::fillCellsDbTable(DbConnector& connector, const std::string& tableName,
		std::vector<std::string> colsNames)
{
	std::vector<std::vector<std::string>> cellsValues;

	for(const auto& cell : m_entities)
	{
		cellsValues.push_back(std::vector<std::string>{ std::to_string(cell->getId()),
			std::to_string(cell->getPosition().column), std::to_string(cell->getPosition().row) });
	}

	SqlMultipleQueryData cellsDataToInsert{tableName, colsNames, cellsValues};
	SqlMultipleInsertQuery cellsInsertQuery(cellsDataToInsert);

	tryToExecuteDbQuery(connector, cellsInsertQuery);
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
