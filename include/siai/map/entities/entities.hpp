#pragma once

#include <memory>
#include <vector>

class PanelPoint;

class IMapEntity;
class MapPosition;

class DbConnector;

namespace Entities
{
    using Pointer = std::shared_ptr<IMapEntity>;
    using Container = std::vector<Pointer>;
    using Iterator = std::vector<Pointer>::iterator;

    void generateMapCells(Container& entities, int numberOfColumns, int numberOfRows);
    void tryToCreateAndAddCell(Container& entities, int id, const MapPosition& position);

    MapPosition findPositionWithPoint(Container& entities, const PanelPoint& point);

    Iterator findCellIteratorWithPosition(Container& entities, const MapPosition& position);
    Iterator findAgvIteratorWithPosition(Container& entities, const MapPosition& position);

    void eraseAgvOnDbWithId(DbConnector& connector, const std::string& mapName, int id);

    Pointer& getEntityByPosition(Entities::Container& entities, const MapPosition& position);
    Pointer& getCellByPosition(Entities::Container& entities, const MapPosition& position);

    void assertPositionInsideMap(const Entities::Container& entities, const MapPosition& position);
    bool isPositionInsideMap(const Entities::Container& entities, const MapPosition& position);

    void assertCellOccupied(const Entities::Container& entities, const MapPosition& position);
    bool isCellOccupied(const Container& entities, const MapPosition& position);

    void assertIsParkingCell(Entities::Container& entities, const MapPosition& position);
    bool isParkingCell(Container& entities, const MapPosition& position);

    bool selectOrDiselectIfHasPointInside(IMapEntity& entity, const PanelPoint& point) noexcept;

    void sortEntitiesByDrawOrder(Container& entities);
}
