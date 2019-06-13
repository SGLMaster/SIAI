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
    ICell::IdManager.reset();

    for(int column = 0; column < numberOfColumns; ++column)
    {
        for(int row = 0; row < numberOfRows; ++row)
        {
            MapPosition position{column, row};

            tryToCreateAndAddCell(entities, ICell::IdManager.getId(), position);
        }
    }
}

void Entities::tryToCreateAndAddCell(Stock& entities, int id, const MapPosition& position)
{
    try
    {
        Entities::CellPtr tmpCell = ICell::create("Regular", id, position);

        entities.all.push_back(tmpCell);
        entities.cells.push_back(tmpCell);
    }
    catch(EntityException& e)
    {
        Log::error(e.what());
        exit(-1);
    }
}

// Map grid generation to be used by the PathFinder algorithm
MapGrid Entities::generateMapGrid(Stock& entities, const MapPosition& source, const MapPosition& destination)
{
    MapGrid mapGrid;
    mapGrid.reserve(entities.cells.size());
    mapGrid.push_back(std::vector<int>());  // Pushing back the first column before entering the loop

    int iterColumn = 0;
    for(const auto& cell : entities.cells)
    {
        int cellColumn = cell->getPosition().column;

        // If the column numbers are different it means we need to add a new one to the map grid
        if(iterColumn != cellColumn)
        {
            mapGrid.push_back(std::vector<int>());
            iterColumn = cellColumn;
        }

        MapPosition cellPosition = cell->getPosition();
        std::string cellType = cell->getEntityName();
        MapDirection cellDirection = cell->getDirection();

        bool thisIsTheSource = (cellPosition.column == source.column) && (cellPosition.row == source.row);
        bool thisIsTheDestination = (cellPosition.column == destination.column) 
                                    && (cellPosition.row == destination.row);

        if(cellType == "Blocked")
            cellDirection = MapDirection::INVALID;
        else if( (cellType == "Parking" || cellType == "Storage") && !thisIsTheSource && !thisIsTheDestination)
            cellDirection = MapDirection::INVALID;
        
        int cellValue = static_cast<int>(cellDirection);
        mapGrid[iterColumn].push_back(cellValue);            
    }

    return mapGrid;
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

        Entities::CellPtr tmpCell = ICell::create(cellType, cellId, cellPosition);
        ICell::IdManager.retrieveId(cellId);
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

        Entities::AgvPtr tmpAgv = IAgv::create("RegularAgv", agvId, agvPosition);
        IAgv::IdManager.retrieveId(agvId);
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

        Entities::RackPtr tmpRack = IRack::create("RegularRack", rackId, rackPosition);
        IRack::IdManager.retrieveId(rackId);
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

void Entities::eraseCell(Stock& entities, int id)
{
    eraseCellInAll(entities.all, id);
    eraseCellInCells(entities.cells, id);
}

void Entities::eraseCellInAll(Container& all, int id)
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

    auto itInAll = std::find_if(all.begin(), all.end(), findInAll);

    if(itInAll == all.end())
	{
		throw EntityNotFound();
	}

    all.erase(itInAll);
}

void Entities::eraseCellInCells(Cells& cells, int id)
{
    auto findInCells = [&id](const Entities::CellPtr& cell){ return cell->getId() == id; };

    auto itInCells = std::find_if(cells.begin(), cells.end(), findInCells);

    if(itInCells == cells.end())
	{
		throw EntityNotFound();
	}

    cells.erase(itInCells);
}

void Entities::eraseAgv(Stock& entities, int id)
{
    eraseAgvInAll(entities.all, id);
    eraseAgvInAgvs(entities.agvs, id);
}

void Entities::eraseAgvInAll(Container& all, int id)
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

    auto itInAll = std::find_if(all.begin(), all.end(), findInAll);

    if(itInAll == all.end())
	{
		throw EntityNotFound();
	}

    all.erase(itInAll);
}

void Entities::eraseAgvInAgvs(Agvs& agvs, int id)
{
    auto findInAgvs = [&id](const Entities::AgvPtr& agv){ return agv->getId() == id; };

    auto itInAgvs = std::find_if(agvs.begin(), agvs.end(), findInAgvs);

    if(itInAgvs == agvs.end())
	{
		throw EntityNotFound();
	}

    agvs.erase(itInAgvs);
}

void Entities::eraseAgv(Stock& entities, const MapPosition& position)
{
    eraseAgvInAll(entities.all, position);
    eraseAgvInAgvs(entities.agvs, position);
}

void Entities::eraseAgvInAll(Container& all, const MapPosition& position)
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

    auto itInAll = std::find_if(all.begin(), all.end(), findInAll);

    if(itInAll == all.end())
	{
		throw EntityNotFound();
	}

    all.erase(itInAll);
}

void Entities::eraseAgvInAgvs(Agvs& agvs, const MapPosition& position)
{
    auto findInAgvs = [&position](const Entities::AgvPtr& agv)
                                { 
                                    return (agv->getPosition().column == position.column 
                                    && agv->getPosition().row == position.row); 
                                };

    auto itInAgvs = std::find_if(agvs.begin(), agvs.end(), findInAgvs);

    if(itInAgvs == agvs.end())
	{
		throw EntityNotFound();
	}

    agvs.erase(itInAgvs);
}

void Entities::eraseRack(Stock& entities, int id)
{
    eraseRackInAll(entities.all, id);
    eraseRackInRacks(entities.racks, id);    
}

void Entities::eraseRackInAll(Container& all, int id)
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

    auto itInAll = std::find_if(all.begin(), all.end(), findInAll);

    if(itInAll == all.end())
	{
		throw EntityNotFound();
	}

    all.erase(itInAll);
}

void Entities::eraseRackInRacks(Racks& racks, int id)
{
    auto findInRacks = [&id](const Entities::RackPtr& rack){ return rack->getId() == id; };

    auto itInRacks = std::find_if(racks.begin(), racks.end(), findInRacks);

    if(itInRacks == racks.end())
	{
		throw EntityNotFound();
	}

    racks.erase(itInRacks);
}

void Entities::eraseRack(Stock& entities, const MapPosition& position)
{
    eraseRackInAll(entities.all, position);
    eraseRackInRacks(entities.racks, position);
}

void Entities::eraseRackInAll(Container& all, const MapPosition& position)
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

    auto itInAll = std::find_if(all.begin(), all.end(), findInAll);

    if(itInAll == all.end())
	{
		throw EntityNotFound();
	}

    all.erase(itInAll);
}

void Entities::eraseRackInRacks(Racks& racks, const MapPosition& position)
{
    auto findInRacks = [&position](const Entities::RackPtr& rack)
                            { 
                                return (rack->getPosition().column == position.column 
                                    && rack->getPosition().row == position.row); 
                            };

    auto itInRacks = std::find_if(racks.begin(), racks.end(), findInRacks);

    if(itInRacks == racks.end())
	{
		throw EntityNotFound();
	}

    racks.erase(itInRacks);
}

void Entities::eraseAgvOnDb(DbConnector& connector, const std::string& mapName, int id)
{
	SqlWhereCondition whereCondition( SqlQueryData{"", {IAgv::primaryKeyName}, {std::to_string(id)} } );
	SqlDeleteRowQuery eraseAgvQuery(SIAIGlobals::DB_AGVS_TABLE_PREFIX + mapName, whereCondition.generateString());

	connector.executeQueryWithoutResults(eraseAgvQuery);
}

void Entities::eraseAgvOnDb(DbConnector& connector, const std::string& mapName, const MapPosition& position)
{
	SqlWhereCondition whereCondition( SqlQueryData{"", {"column", "row"}, {std::to_string(position.column), 
                                                                            std::to_string(position.row)} } );
	SqlDeleteRowQuery eraseAgvQuery(SIAIGlobals::DB_AGVS_TABLE_PREFIX + mapName, whereCondition.generateString());

	connector.executeQueryWithoutResults(eraseAgvQuery);
}

void Entities::eraseRackOnDb(DbConnector& connector, const std::string& mapName, int id)
{
	SqlWhereCondition whereCondition( SqlQueryData{"", {IRack::primaryKeyName}, {std::to_string(id)} } );
	SqlDeleteRowQuery eraseRackQuery(SIAIGlobals::DB_RACKS_TABLE_PREFIX + mapName, whereCondition.generateString());

	connector.executeQueryWithoutResults(eraseRackQuery);
}

void Entities::eraseRackOnDb(DbConnector& connector, const std::string& mapName, const MapPosition& position)
{
	SqlWhereCondition whereCondition( SqlQueryData{"", {"column", "row"}, {std::to_string(position.column), 
                                                                            std::to_string(position.row)} } );
	SqlDeleteRowQuery eraseRackQuery(SIAIGlobals::DB_RACKS_TABLE_PREFIX + mapName, whereCondition.generateString());

	connector.executeQueryWithoutResults(eraseRackQuery);
}

Entities::CellPtr Entities::getCellWithId(Entities::Cells& cells, int id)
{
    auto findCellWithId = [&id](const Entities::CellPtr& cell){ return cell->getId() == id; };

    auto cellFound = std::find_if(cells.begin(), cells.end(), findCellWithId);

    if(cellFound == cells.end())
	{
		return nullptr;
	}

    return *cellFound;
}

Entities::AgvPtr Entities::getAgvWithId(Entities::Agvs& agvs, int id)
{
    auto findAgvWithId = [&id](const Entities::AgvPtr& agv){ return agv->getId() == id; };

    auto agvFound = std::find_if(agvs.begin(), agvs.end(), findAgvWithId);

    if(agvFound == agvs.end())
	{
		return nullptr;
	}

    return *agvFound;
}

Entities::Pointer Entities::getEntityByPosition(Entities::Stock& entities, const MapPosition& position)
{
	auto findFirstEntityInPosition = [&position](Entities::Pointer& entity)
	                                    {
	                                        return (entity->getPosition().column == position.column
	                                                && entity->getPosition().row == position.row);
	                                    };

	auto entityFound = std::find_if(entities.all.rbegin(), entities.all.rend(), findFirstEntityInPosition);

	if(entityFound == entities.all.rend())
	{
		return nullptr;
	}

	return *entityFound;
}

Entities::CellPtr Entities::getCellByPosition(Entities::Cells& cells, const MapPosition& position)
{
	auto findCellInPosition = [&position](Entities::CellPtr& cell)
                                        {
	                                        return (cell->getPosition().column == position.column
	                                                && cell->getPosition().row == position.row);
	                                    };

	auto cellFound = std::find_if(cells.rbegin(), cells.rend(), findCellInPosition);

	if(cellFound == cells.rend())
	{
		return nullptr;
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
