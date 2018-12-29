#include "map/entities/entities.hpp"
#include "map/entities/mapentity.hpp"

#include <algorithm>

void Entities::generateMapCells(Container& entities, int numberOfColumns, int numberOfRows)
{
    int idGenerator{0};

    for(int column = 0; column < numberOfColumns; ++column)
    {
        for(int row = 0; row < numberOfRows; ++row)
        {
            int id = ++idGenerator;
            MapPosition position{column, row, MapDirection::RIGHT};

            Entities::Pointer tmpCell = ICell::create("RegularCell", id, position);

            entities.push_back(std::move(tmpCell));

            ///////////////////////////TEST//////////////////////////
            if(position.column < 20)
            {
                Entities::Pointer tmpAgv = IAgv::create("RegularAgv", id, position);
                entities.push_back(std::move(tmpAgv));
            }

        }
    }
}

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
