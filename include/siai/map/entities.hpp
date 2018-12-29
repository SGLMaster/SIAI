#pragma once

#include <memory>
#include <vector>

class PanelPoint;

class IMapEntity;
class MapPosition;

namespace Entities
{
    using Pointer = std::unique_ptr<IMapEntity>;
    using Container = std::vector<Pointer>;
    using Iterator = std::vector<Pointer>::iterator;

    Iterator findCellIteratorWithPoint(Container& entities, const PanelPoint& point);

    bool isCellOccupied(const Container& entities, const MapPosition& position);

    void createCellCopyWithDifferentType(Container& entities, Iterator& it, const std::string& type);

    void sortEntitiesByDrawOrder(Container& entities);
}
