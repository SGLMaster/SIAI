#pragma once

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
    void tryToCreateAndAddCell(Container& entities, const MapPosition& position);

    Iterator findCellIteratorWithPoint(Container& entities, const PanelPoint& point);
    Iterator findCellIteratorWithPosition(Container& entities, const MapPosition& position);
    Iterator findAgvIteratorWithPosition(Container& entities, const MapPosition& position);

    MapPosition findPositionWithPoint(Container& entities, const PanelPoint& point);

    void assertCellOccupied(const Entities::Container& entities, const MapPosition& position);

    bool isCellOccupied(const Container& entities, const MapPosition& position);

    bool selectOrDiselectIfHasPointInside(IMapEntity& entity, const PanelPoint& point) noexcept;

    void createCellCopyWithDifferentType(Container& entities, Iterator& it, const std::string& type);

    void sortEntitiesByDrawOrder(Container& entities);
}
