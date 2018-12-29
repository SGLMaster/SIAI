#include "map/entities.hpp"
#include "map/mapentity.hpp"

#include <algorithm>

Entities::Iterator Entities::findCellIteratorWithPoint(Container& entities, const PanelPoint& point)
{
    auto findCellWithPointInside = [&point](const Entities::Pointer& entity)
                                    {
                                        bool entityIsACell = dynamic_cast<ICell*>(entity.get()) != nullptr;

                                        return (entity->hasPointInside(point)
                                                && entityIsACell);
                                    };

    return std::find_if(entities.begin(), entities.end(), findCellWithPointInside);
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

void Entities::createCellCopyWithDifferentType(Container& entities, Iterator& it, const std::string& type)
{
    int newId = (*it)->getId();
    MapPosition newPosition = (*it)->getPosition();

    entities.push_back(ICell::create( type, newId, newPosition ));
}

void Entities::sortEntitiesByDrawOrder(Container& entities)
{
    auto drawOrderSorting = [](const Entities::Pointer& entityLhs, const Entities::Pointer& entityRhs)
                            {
                                    return (entityLhs->getDrawOrder() < entityRhs->getDrawOrder());
                            };

    std::stable_sort(entities.begin(), entities.end(), drawOrderSorting);
}
