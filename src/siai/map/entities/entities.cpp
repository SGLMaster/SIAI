#include "map/entities/entities.hpp"
#include "map/entities/cell.hpp"
#include "map/entities/agv.hpp"
#include "map/entities/rack.hpp"
#include "map/exception.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "util/id.hpp"

#include "globals.hpp"
#include "log.hpp"

#include <mysql++.h>

#include <algorithm>

void Entities::generateMapCells(Stock& entities, int numberOfColumns, int numberOfRows)
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

void Entities::tryToCreateAndAddCell(Stock& entities, int id, const MapPosition& position)
{
    try
    {
        Entities::Pointer tmpCell = ICell::create("Regular", id, position);

        entities.all.push_back(tmpCell);
        entities.cells.push_back(tmpCell);
    }
    catch(EntityException& e)
    {
        Log::error(e.what());
        exit(-1);
    }
}

void Entities::loadCellsFromQueryRows(Stock& entities, const std::vector<DbRow>& rows)
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

        entities.all.push_back(tmpCell);
        entities.cells.push_back(tmpCell);
    }
}

void Entities::loadAgvsFromQueryRows(Stock& entities, const std::vector<DbRow>& rows)
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

        entities.all.push_back(tmpAgv);
        entities.agvs.push_back(tmpAgv);
    }
}

void Entities::loadRacksFromQueryRows(Stock& entities, const std::vector<DbRow>& rows)
{
    for (const DbRow& rackDataRow : rows) 
    {
        int rackId = rackDataRow[0];
        int rackCol = rackDataRow[1];
        int rackRow = rackDataRow[2];
        int rackDirection = rackDataRow[3];

        MapPosition rackPosition{rackCol, rackRow};

        Entities::Pointer tmpRack = IRack::create("RegularRack", rackId, rackPosition);
        IRack::RacksIdManager.retrieveId(rackId);
        tmpRack->setDirection(static_cast<MapDirection>(rackDirection));

        entities.all.push_back(tmpRack);
        entities.racks.push_back(tmpRack);
    }
}

void Entities::updateAgvsFromQueryRows(Stock& entities, const std::vector<DbRow>& rows)
{
    for (const DbRow& agvDataRow : rows) 
    {
        int agvId = agvDataRow[0];
        int agvCol = agvDataRow[1];
        int agvRow = agvDataRow[2];
        int agvDirectionValue = agvDataRow[3];

        MapDirection agvDirection = static_cast<MapDirection>(agvDirectionValue);
        MapPosition agvPosition{agvCol, agvRow};
        
        auto agvFound = getAgvWithId(entities.agvs, agvId);

        agvFound->setPosition(agvPosition);
        agvFound->setDirection(agvDirection);
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

Entities::Iterator Entities::findCellIteratorWithPosition(Stock& entities, const MapPosition& position)
{
    auto findCellInPosition = [&position](const Entities::Pointer& entity)
                                    {
                                        bool entityIsACell = dynamic_cast<ICell*>(entity.get()) != nullptr;

                                        return (entity->getPosition().column == position.column
                                                && entity->getPosition().row == position.row
                                                && entityIsACell);
                                    };

    Entities::Iterator cellFound = std::find_if(entities.all.begin(), entities.all.end(), findCellInPosition);

    if(cellFound == entities.all.end())
    {
        throw EntityNotFound();
    }

    return cellFound;
}

MapPosition Entities::findPositionWithPoint(Stock& entities, const PanelPoint& point)
{
    auto findEntityWithPointInside = [&point](const Entities::Pointer& entity)
                                    {
                                        return entity->hasPointInside(point);
                                    };

    auto entityFound = std::find_if(entities.all.begin(), entities.all.end(), findEntityWithPointInside);

    if(entityFound == entities.all.end())
    {
        throw EntityNotFound();
    }

    return (*entityFound)->getPosition();
}

Entities::Iterator Entities::findAgvIteratorWithPosition(Stock& entities, const MapPosition& position)
{
    auto findAgvInPosition = [agvPosition = position](const Entities::Pointer& entity)
                                    {
                                        bool entityIsAnAgv = dynamic_cast<IAgv*>(entity.get()) != nullptr;

                                        return entity->getPosition().column == agvPosition.column
                                                && entity->getPosition().row == agvPosition.row
                                                && entityIsAnAgv;
                                    };

    Entities::Iterator agvFound = std::find_if(entities.all.begin(), entities.all.end(), findAgvInPosition);

    if(agvFound == entities.all.end())
    {
        throw EntityNotFound();
    }

    return agvFound;
}

Entities::Iterator Entities::findRackIteratorWithPosition(Stock& entities, const MapPosition& position)
{
    auto findRackInPosition = [rackPosition = position](const Entities::Pointer& entity)
                                    {
                                        bool entityIsARack = dynamic_cast<IRack*>(entity.get()) != nullptr;

                                        return entity->getPosition().column == rackPosition.column
                                                && entity->getPosition().row == rackPosition.row
                                                && entityIsARack;
                                    };

    Entities::Iterator rackFound = std::find_if(entities.all.begin(), entities.all.end(), findRackInPosition);

    if(rackFound == entities.all.end())
    {
        throw EntityNotFound();
    }

    return rackFound;
}

void Entities::eraseCell(Stock& entities, int id)
{
    auto findInAll = [&id](const Entities::Pointer& entity)
                        {
                            if(entity->getId() == id)
                            {
                                bool entityIsCell = dynamic_cast<ICell*>(entity.get()) != nullptr;
                                if(entityIsCell)
                                    return true;
                            }
                                    
                            return false;
                        };

    auto itInAll = std::find_if(entities.all.begin(), entities.all.end(), findInAll);

    if(itInAll == entities.all.end())
	{
		throw EntityNotFound();
	}

    entities.all.erase(itInAll);

    auto findInCells = [&id](const Entities::Pointer& cell){ return cell->getId() == id; };

    auto itInCells = std::find_if(entities.cells.begin(), entities.cells.end(), findInCells);

    if(itInCells == entities.cells.end())
	{
		throw EntityNotFound();
	}

    entities.cells.erase(itInCells);
}

void Entities::eraseAgv(Stock& entities, int id)
{
    auto findInAll = [&id](const Entities::Pointer& entity)
                        {
                            if(entity->getId() == id)
                            {
                                bool entityIsAgv = dynamic_cast<IAgv*>(entity.get()) != nullptr;
                                if(entityIsAgv)
                                    return true;
                            }
                                    
                            return false;
                        };

    auto itInAll = std::find_if(entities.all.begin(), entities.all.end(), findInAll);

    if(itInAll == entities.all.end())
	{
		throw EntityNotFound();
	}

    entities.all.erase(itInAll);

    auto findInAgvs = [&id](const Entities::Pointer& agv){ return agv->getId() == id; };

    auto itInAgvs = std::find_if(entities.agvs.begin(), entities.agvs.end(), findInAgvs);

    if(itInAgvs == entities.agvs.end())
	{
		throw EntityNotFound();
	}

    entities.agvs.erase(itInAgvs);
}

void Entities::eraseRack(Stock& entities, int id)
{
    auto findInAll = [&id](const Entities::Pointer& entity)
                        {
                            if(entity->getId() == id)
                            {
                                bool entityIsRack = dynamic_cast<IRack*>(entity.get()) != nullptr;
                                if(entityIsRack)
                                    return true;
                            }
                                    
                            return false;
                        };

    auto itInAll = std::find_if(entities.all.begin(), entities.all.end(), findInAll);

    if(itInAll == entities.all.end())
	{
		throw EntityNotFound();
	}

    entities.all.erase(itInAll);

    auto findInRacks = [&id](const Entities::Pointer& rack){ return rack->getId() == id; };

    auto itInRacks = std::find_if(entities.racks.begin(), entities.racks.end(), findInRacks);

    if(itInRacks == entities.racks.end())
	{
		throw EntityNotFound();
	}

    entities.racks.erase(itInRacks);
}

void Entities::eraseAgv(Stock& entities, const MapPosition& position)
{
    auto findInAll = [&position](const Entities::Pointer& entity)
                        {
                            if(entity->getPosition().column == position.column 
                                && entity->getPosition().row == position.row)
                            {
                                bool entityIsAgv = dynamic_cast<IAgv*>(entity.get()) != nullptr;
                                if(entityIsAgv)
                                    return true;
                            }
                                    
                            return false;
                        };

    auto itInAll = std::find_if(entities.all.begin(), entities.all.end(), findInAll);

    if(itInAll == entities.all.end())
	{
		throw EntityNotFound();
	}

    entities.all.erase(itInAll);

    auto findInAgvs = [&position](const Entities::Pointer& agv)
                                { 
                                    return (agv->getPosition().column == position.column 
                                    && agv->getPosition().row == position.row); 
                                };

    auto itInAgvs = std::find_if(entities.agvs.begin(), entities.agvs.end(), findInAgvs);

    if(itInAgvs == entities.agvs.end())
	{
		throw EntityNotFound();
	}

    entities.agvs.erase(itInAgvs);
}

void Entities::eraseRack(Stock& entities, const MapPosition& position)
{
    auto findInAll = [&position](const Entities::Pointer& entity)
                        {
                            if(entity->getPosition().column == position.column 
                                && entity->getPosition().row == position.row)
                            {
                                bool entityIsRack = dynamic_cast<IRack*>(entity.get()) != nullptr;
                                if(entityIsRack)
                                    return true;
                            }
                                    
                            return false;
                        };

    auto itInAll = std::find_if(entities.all.begin(), entities.all.end(), findInAll);

    if(itInAll == entities.all.end())
	{
		throw EntityNotFound();
	}

    entities.all.erase(itInAll);

    auto findInRacks = [&position](const Entities::Pointer& rack)
                            { 
                                return (rack->getPosition().column == position.column 
                                    && rack->getPosition().row == position.row); 
                            };

    auto itInRacks = std::find_if(entities.racks.begin(), entities.racks.end(), findInRacks);

    if(itInRacks == entities.racks.end())
	{
		throw EntityNotFound();
	}

    entities.racks.erase(itInRacks);
}

void Entities::eraseAgvOnDb(DbConnector& connector, const std::string& mapName, int id)
{
	SqlWhereCondition whereCondition( SqlQueryData{"", {IAgv::primaryKeyName}, {std::to_string(id)} } );
	SqlDeleteRowQuery eraseAgvQuery(SIAIGlobals::DB_AGVS_TABLE_PREFIX + mapName, whereCondition.generateString());

	connector.executeQueryWithoutResults(eraseAgvQuery);
}

void Entities::eraseRackOnDb(DbConnector& connector, const std::string& mapName, int id)
{
	SqlWhereCondition whereCondition( SqlQueryData{"", {IRack::primaryKeyName}, {std::to_string(id)} } );
	SqlDeleteRowQuery eraseRackQuery(SIAIGlobals::DB_RACKS_TABLE_PREFIX + mapName, whereCondition.generateString());

	connector.executeQueryWithoutResults(eraseRackQuery);
}

Entities::Pointer& Entities::getCellWithId(Entities::Container& cells, int id)
{
    auto findCellWithId = [&id](const Entities::Pointer& cell){ return cell->getId() == id; };

    auto cellFound = std::find_if(cells.begin(), cells.end(), findCellWithId);

    if(cellFound == cells.end())
	{
		throw EntityNotFound();
	}

    return *cellFound;
}

Entities::Pointer& Entities::getAgvWithId(Entities::Container& agvs, int id)
{
    auto findAgvWithId = [&id](const Entities::Pointer& agv){ return agv->getId() == id; };

    auto agvFound = std::find_if(agvs.begin(), agvs.end(), findAgvWithId);

    if(agvFound == agvs.end())
	{
		throw EntityNotFound();
	}

    return *agvFound;
}

Entities::Pointer& Entities::getEntityByPosition(Entities::Stock& entities, const MapPosition& position)
{
	auto findFirstEntityInPosition = [&position](Entities::Pointer& entity)
	                                    {
	                                        return (entity->getPosition().column == position.column
	                                                && entity->getPosition().row == position.row);
	                                    };

	auto entityFound = std::find_if(entities.all.rbegin(), entities.all.rend(), findFirstEntityInPosition);

	if(entityFound == entities.all.rend())
	{
		throw EntityNotFound();
	}

	return *entityFound;
}

Entities::Pointer& Entities::getCellByPosition(Entities::Container& cells, const MapPosition& position)
{
	auto findCellInPosition = [&position](Entities::Pointer& cell)
                                        {
	                                        return (cell->getPosition().column == position.column
	                                                && cell->getPosition().row == position.row);
	                                    };

	auto cellFound = std::find_if(cells.rbegin(), cells.rend(), findCellInPosition);

	if(cellFound == cells.rend())
	{
		throw EntityNotFound();
	}

	return *cellFound;
}

void Entities::assertPositionInsideMap(const Entities::Stock& entities, const MapPosition& position)
{
	bool foundAnyEntityInThisPosition = isPositionInsideMap(entities, position);

	if(!foundAnyEntityInThisPosition)
	{
		throw CellNotFoundException();
	}
}

bool Entities::isPositionInsideMap(const Entities::Stock& entities, const MapPosition& position)
{
	auto findAnyEntityWithPosition = [&position](const Entities::Pointer& entity)
	                                 {
										return (entity->getPosition().column == position.column
												&& entity->getPosition().row == position.row);
	                                 };

	auto entityIterator = std::find_if(entities.all.begin(), entities.all.end(), findAnyEntityWithPosition);

	if(entityIterator == entities.all.end())
	{
		return false;
	}

	return true;
}

void Entities::assertCellOccupied(const Stock& entities, const MapPosition& position)
{
    bool cellFoundIsOccupied = isCellOccupied(entities, position);

    if(cellFoundIsOccupied)
    {
        throw CellOccupiedException();
    }
}

bool Entities::isCellOccupied(const Stock& entities, const MapPosition& position)
{
    auto findADifferentEntityWithPosition = [&position](const Entities::Pointer& entity)
                                            {
                                                bool entityIsNotACell = dynamic_cast<ICell*>(entity.get()) == nullptr;

                                                return (entity->getPosition().column == position.column
                                                        && entity->getPosition().row == position.row
                                                        && entityIsNotACell);
                                            };

    auto entityIterator = std::find_if(entities.all.begin(), entities.all.end(), findADifferentEntityWithPosition);

    if(entityIterator == entities.all.end())
    {
        return false;
    }

    return true;
}

void Entities::assertIsParkingCell(Stock& entities, const MapPosition& position)
{
    bool cellFoundIsParking = isParkingCell(entities, position);

    if(!cellFoundIsParking)
    {
        throw NotParkingCellException();
    }
}

bool Entities::isParkingCell(Stock& entities, const MapPosition& position)
{
    Pointer cellToAssert = getCellByPosition(entities.cells, position);

    if(cellToAssert->getEntityName() == std::string("Parking"))
    {
        return true;
    }
    return false;
}

void Entities::assertIsStorageCell(Stock& entities, const MapPosition& position)
{
    bool cellFoundIsStorage = isStorageCell(entities, position);

    if(!cellFoundIsStorage)
    {
        throw NotStorageCellException();
    }
}

bool Entities::isStorageCell(Stock& entities, const MapPosition& position)
{
    Pointer cellToAssert = getCellByPosition(entities.cells, position);

    if(cellToAssert->getEntityName() == std::string("Storage"))
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

void Entities::sortEntitiesByDrawOrder(Stock& entities)
{
    auto drawOrderSorting = [](const Entities::Pointer& entityLhs, const Entities::Pointer& entityRhs)
                            {
                                    return (entityLhs->getDrawOrder() < entityRhs->getDrawOrder());
                            };

    std::stable_sort(entities.all.begin(), entities.all.end(), drawOrderSorting);
}
