#include "map/entities/entities.hpp"
#include "map/entities/cell.hpp"
#include "map/entities/agv.hpp"
#include "map/exception.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "util/id.hpp"

#include "globals.hpp"
#include "log.hpp"

#include <mysql++.h>

#include <algorithm>

void Entities::generateMapCells(Container& entities, int numberOfColumns, int numberOfRows)
{
    ICell::CellsIdManager.reset();

    for(int column = 0; column < numberOfColumns; ++column)
    {
        for(int row = 0; row < numberOfRows; ++row)
        {
            MapPosition position{column, row};

            tryToCreateAndAddCell(entities, ICell::CellsIdManager.getId(), position);
        }
    }
}

void Entities::tryToCreateAndAddCell(Container& entities, int id, const MapPosition& position)
{
    try
    {
        Entities::Pointer tmpCell = ICell::create("Regular", id, position);

        entities.push_back(std::move(tmpCell));
    }
    catch(EntityException& e)
    {
        Log::error(e.what());
        exit(-1);
    }
}

void Entities::loadCellsFromQueryRows(Container& entities, const std::vector<DbRow>& rows)
{
    for (const DbRow& cellDataRow : rows) 
    {
        int cellId = cellDataRow[0];
        int cellCol = cellDataRow[1];
        int cellRow = cellDataRow[2];
        int cellDirection = cellDataRow[3];
        std::string cellType(cellDataRow[4]);

        MapPosition cellPosition{cellCol, cellRow};

        Entities::Pointer tmpCell = ICell::create(cellType, cellId, cellPosition);
        ICell::CellsIdManager.retrieveId(cellId);
        tmpCell->setDirection(static_cast<MapDirection>(cellDirection));

        entities.push_back(std::move(tmpCell));
    }
}

void Entities::loadAgvsFromQueryRows(Container& entities, const std::vector<DbRow>& rows)
{
    for (const DbRow& agvDataRow : rows) 
    {
        int agvId = agvDataRow[0];
        int agvCol = agvDataRow[1];
        int agvRow = agvDataRow[2];
        int agvDirection = agvDataRow[3];

        MapPosition agvPosition{agvCol, agvRow};

        Entities::Pointer tmpAgv = IAgv::create("RegularAgv", agvId, agvPosition);
        IAgv::AgvsIdManager.retrieveId(agvId);
        tmpAgv->setDirection(static_cast<MapDirection>(agvDirection));

        entities.push_back(std::move(tmpAgv));
    }
}

int Entities::getNumberOfMapColsFromDbRows(const std::vector<DbRow>& rows)
{
    int numberOfCols = 0;

    for (const DbRow& cellDataRow : rows) 
    {
        int cellCol = cellDataRow[1];

        numberOfCols = (cellCol>numberOfCols) ? cellCol : numberOfCols;
    }

    return ++numberOfCols; //Since the cols are numbered starting from 0, we must sum 1 to get the total of cols
}

int Entities::getNumberOfMapRowsFromDbRows(const std::vector<DbRow>& rows)
{
    int numberOfRows = 0;

    for (const DbRow& cellDataRow : rows) 
    {
        int cellRow = cellDataRow[2];

        numberOfRows = (cellRow>numberOfRows) ? cellRow : numberOfRows;
    }

    return ++numberOfRows; //Since the rows are numbered starting from 0, we must sum 1 to get the total of rows
}

Entities::Iterator Entities::findCellIteratorWithPosition(Container& entities, const MapPosition& position)
{
    auto findCellInPosition = [&position](const Entities::Pointer& entity)
                                    {
                                        bool entityIsACell = dynamic_cast<ICell*>(entity.get()) != nullptr;

                                        return (entity->getPosition().column == position.column
                                                && entity->getPosition().row == position.row
                                                && entityIsACell);
                                    };

    Entities::Iterator cellFound = std::find_if(entities.begin(), entities.end(), findCellInPosition);

    if(cellFound == entities.end())
    {
        throw EntityNotFound();
    }

    return cellFound;
}

MapPosition Entities::findPositionWithPoint(Container& entities, const PanelPoint& point)
{
    auto findEntityWithPointInside = [&point](const Entities::Pointer& entity)
                                    {
                                        return entity->hasPointInside(point);
                                    };

    auto entityFound = std::find_if(entities.begin(), entities.end(), findEntityWithPointInside);

    if(entityFound == entities.end())
    {
        throw EntityNotFound();
    }

    return (*entityFound)->getPosition();
}

Entities::Iterator Entities::findAgvIteratorWithPosition(Container& entities, const MapPosition& position)
{
    auto findAgvInPosition = [agvPosition = position](const Entities::Pointer& entity)
                                    {
                                        bool entityIsAnAgv = dynamic_cast<IAgv*>(entity.get()) != nullptr;

                                        return entity->getPosition().column == agvPosition.column
                                                && entity->getPosition().row == agvPosition.row
                                                && entityIsAnAgv;
                                    };

    Entities::Iterator agvFound = std::find_if(entities.begin(), entities.end(), findAgvInPosition);

    if(agvFound == entities.end())
    {
        throw EntityNotFound();
    }

    return agvFound;
}

void Entities::eraseAgvOnDbWithId(DbConnector& connector, const std::string& mapName, int id)
{
	SqlWhereCondition whereCondition( SqlQueryData{"", {IAgv::primaryKeyName}, {std::to_string(id)} } );
	SqlDeleteRowQuery eraseAgvQuery(SIAIGlobals::DB_AGVS_TABLE_PREFIX + mapName, whereCondition.generateString());

	connector.executeQueryWithoutResults(eraseAgvQuery);
}

Entities::Pointer& Entities::getEntityByPosition(Entities::Container& entities, const MapPosition& position)
{
	auto findFirstEntityInPosition = [&position](Entities::Pointer& entity)
	                                    {
	                                        return (entity->getPosition().column == position.column
	                                                && entity->getPosition().row == position.row);
	                                    };

	auto entityFound = std::find_if(entities.rbegin(), entities.rend(), findFirstEntityInPosition);

	if(entityFound == entities.rend())
	{
		throw EntityNotFound();
	}

	return *entityFound;
}

Entities::Pointer& Entities::getCellByPosition(Entities::Container& entities, const MapPosition& position)
{
	auto findCellInPosition = [&position](Entities::Pointer& entity)
	                                    {
                                            bool entityIsACell = dynamic_cast<ICell*>(entity.get()) != nullptr;

	                                        return (entity->getPosition().column == position.column
	                                                && entity->getPosition().row == position.row 
                                                    && entityIsACell);
	                                    };

	auto cellFound = std::find_if(entities.rbegin(), entities.rend(), findCellInPosition);

	if(cellFound == entities.rend())
	{
		throw EntityNotFound();
	}

	return *cellFound;
}

void Entities::assertPositionInsideMap(const Entities::Container& entities, const MapPosition& position)
{
	bool foundAnyEntityInThisPosition = isPositionInsideMap(entities, position);

	if(!foundAnyEntityInThisPosition)
	{
		throw CellNotFoundException();
	}
}

bool Entities::isPositionInsideMap(const Entities::Container& entities, const MapPosition& position)
{
	auto findAnyEntityWithPosition = [&position](const Entities::Pointer& entity)
	                                 {
										return (entity->getPosition().column == position.column
												&& entity->getPosition().row == position.row);
	                                 };

	auto entityIterator = std::find_if(entities.begin(), entities.end(), findAnyEntityWithPosition);

	if(entityIterator == entities.end())
	{
		return false;
	}

	return true;
}

void Entities::assertCellOccupied(const Container& entities, const MapPosition& position)
{
    bool cellFoundIsOccupied = isCellOccupied(entities, position);

    if(cellFoundIsOccupied)
    {
        throw CellOccupiedException();
    }
}

bool Entities::isCellOccupied(const Container& entities, const MapPosition& position)
{
    auto findADifferentEntityWithPosition = [&position](const Entities::Pointer& entity)
                                            {
                                                bool entityIsNotACell = dynamic_cast<ICell*>(entity.get()) == nullptr;

                                                return (entity->getPosition().column == position.column
                                                        && entity->getPosition().row == position.row
                                                        && entityIsNotACell);
                                            };

    auto entityIterator = std::find_if(entities.begin(), entities.end(), findADifferentEntityWithPosition);

    if(entityIterator == entities.end())
    {
        return false;
    }

    return true;
}

void Entities::assertIsParkingCell(Container& entities, const MapPosition& position)
{
    bool cellFoundIsParking = isParkingCell(entities, position);

    if(!cellFoundIsParking)
    {
        throw NotParkingCellException();
    }
}

bool Entities::isParkingCell(Container& entities, const MapPosition& position)
{
    Pointer cellToAssert = getCellByPosition(entities, position);

    if(cellToAssert->getEntityName() == std::string("Parking"))
    {
        return true;
    }
    return false;
}

bool Entities::selectOrDiselectIfHasPointInside(IMapEntity& entity, const PanelPoint& point) noexcept
{
    constexpr bool someEntityChanged{true};
    constexpr bool noEntityChanged{false};

    if(entity.hasPointInside(point))
    {
        if(!entity.isSelected())
            entity.select();
        else
            entity.diselect();
        return someEntityChanged;
    }

    return noEntityChanged;
}

void Entities::sortEntitiesByDrawOrder(Container& entities)
{
    auto drawOrderSorting = [](const Entities::Pointer& entityLhs, const Entities::Pointer& entityRhs)
                            {
                                    return (entityLhs->getDrawOrder() < entityRhs->getDrawOrder());
                            };

    std::stable_sort(entities.begin(), entities.end(), drawOrderSorting);
}
