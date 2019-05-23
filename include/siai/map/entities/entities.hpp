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

    void generateMapCells(Container& entities, int numberOfColumns, int numberOfRows);
    void tryToCreateAndAddCell(Container& entities, int id, const MapPosition& position);

    void loadCellsFromQueryRows(Container& entities, const std::vector<DbRow>& rows);
    void loadAgvsFromQueryRows(Container& entities, const std::vector<DbRow>& rows);
    void loadRacksFromQueryRows(Container& entities, const std::vector<DbRow>& rows);

    void updateAgvsFromQueryRows(Container& entities, const std::vector<DbRow>& rows);

    int getNumberOfMapColsFromDbRows(const std::vector<DbRow>& rows);
    int getNumberOfMapRowsFromDbRows(const std::vector<DbRow>& rows);

    MapPosition findPositionWithPoint(Container& entities, const PanelPoint& point);

    Iterator findCellIteratorWithPosition(Container& entities, const MapPosition& position);
    Iterator findAgvIteratorWithPosition(Container& entities, const MapPosition& position);

    void eraseAgvOnDbWithId(DbConnector& connector, const std::string& mapName, int id);

    Pointer& getAgvWithId(Entities::Container& entities, int id);
    Pointer& getCellWithId(Entities::Container& entities, int id);

    Pointer& getEntityByPosition(Entities::Container& entities, const MapPosition& position);
    Pointer& getCellByPosition(Entities::Container& entities, const MapPosition& position);

    void assertPositionInsideMap(const Entities::Container& entities, const MapPosition& position);
    bool isPositionInsideMap(const Entities::Container& entities, const MapPosition& position);

    void assertCellOccupied(const Entities::Container& entities, const MapPosition& position);
    bool isCellOccupied(const Container& entities, const MapPosition& position);

    void assertIsParkingCell(Entities::Container& entities, const MapPosition& position);
    bool isParkingCell(Container& entities, const MapPosition& position);

    void assertIsStorageCell(Entities::Container& entities, const MapPosition& position);
    bool isStorageCell(Container& entities, const MapPosition& position);

    bool selectOrDiselectIfHasPointInside(IMapEntity& entity, const PanelPoint& point) noexcept;

    void sortEntitiesByDrawOrder(Container& entities);
}
