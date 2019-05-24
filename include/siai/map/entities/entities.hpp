#pragma once

#include "database/database.hpp"

#include <memory>
#include <vector>

class PanelPoint;

class IMapEntity;
class MapPosition;

namespace Entities
{
    using Pointer = std::shared_ptr<IMapEntity>;
    using Container = std::vector<Pointer>;
    using Iterator = std::vector<Pointer>::iterator;

    struct Stock
    {
        Entities::Container all;
        Entities::Container cells;
        Entities::Container agvs;
        Entities::Container racks;
    };

    void generateMapCells(Stock& entities, int numberOfColumns, int numberOfRows);
    void tryToCreateAndAddCell(Stock& entities, int id, const MapPosition& position);

    void loadCellsFromQueryRows(Stock& entities, const std::vector<DbRow>& rows);
    void loadAgvsFromQueryRows(Stock& entities, const std::vector<DbRow>& rows);
    void loadRacksFromQueryRows(Stock& entities, const std::vector<DbRow>& rows);

    void updateAgvsFromQueryRows(Stock& entities, const std::vector<DbRow>& rows);

    int getNumberOfMapColsFromDbRows(const std::vector<DbRow>& rows);
    int getNumberOfMapRowsFromDbRows(const std::vector<DbRow>& rows);

    MapPosition findPositionWithPoint(Stock& entities, const PanelPoint& point);

    Iterator findCellIteratorWithPosition(Stock& entities, const MapPosition& position);
    Iterator findAgvIteratorWithPosition(Stock& entities, const MapPosition& position);
    Iterator findRackIteratorWithPosition(Stock& entities, const MapPosition& position);

    void eraseCell(Stock& entities, int id);
    void eraseAgv(Stock& entities, int id);
    void eraseRack(Stock& entities, int id);

    void eraseAgv(Stock& entities, const MapPosition& position);
    void eraseRack(Stock& entities, const MapPosition& position);

    void eraseAgvOnDb(DbConnector& connector, const std::string& mapName, int id);
    void eraseRackOnDb(DbConnector& connector, const std::string& mapName, int id);

    void eraseAgvOnDb(DbConnector& connector, const std::string& mapName, const MapPosition& position);
    void eraseRackOnDb(DbConnector& connector, const std::string& mapName, const MapPosition& position);

    Pointer& getCellWithId(Container& cells, int id);
    Pointer& getAgvWithId(Container& agvs, int id);

    Pointer& getEntityByPosition(Stock& entities, const MapPosition& position);
    Pointer& getCellByPosition(Container& cells, const MapPosition& position);

    void assertPositionInsideMap(const Stock& entities, const MapPosition& position);
    bool isPositionInsideMap(const Stock& entities, const MapPosition& position);

    void assertCellOccupied(const Stock& entities, const MapPosition& position);
    bool isCellOccupied(const Stock& entities, const MapPosition& position);

    void assertIsParkingCell(Stock& entities, const MapPosition& position);
    bool isParkingCell(Stock& entities, const MapPosition& position);

    void assertIsStorageCell(Stock& entities, const MapPosition& position);
    bool isStorageCell(Stock& entities, const MapPosition& position);

    bool selectOrDiselectIfHasPointInside(IMapEntity& entity, const PanelPoint& point) noexcept;

    void sortEntitiesByDrawOrder(Stock& entities);
}
