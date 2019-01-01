#include "map/entities/entities.hpp"
#include "map/entities/mapentity.hpp"

#include "map/exception.hpp"
#include "log.hpp"

#include <algorithm>

void Entities::generateMapCells(Container& entities, int numberOfColumns, int numberOfRows)
{
    ICell::CellsIdManager.reset();

    for(int column = 0; column < numberOfColumns; ++column)
    {
        for(int row = 0; row < numberOfRows; ++row)
        {
            MapPosition position{column, row};

            tryToCreateAndAddCell(entities, position);
        }
    }
}

void Entities::tryToCreateAndAddCell(Container& entities, const MapPosition& position)
{
    try
    {
        Entities::Pointer tmpCell = ICell::create("RegularCell", ICell::CellsIdManager.getId(), position);

        entities.push_back(std::move(tmpCell));
    }
    catch(EntityException& e)
    {
        Log::fatalError(e.what());
        exit(-1);
    }
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

void Entities::assertCellOccupied(const Entities::Container& entities, const MapPosition& position)
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

    entities.push_back(ICell::create(type, newId, newPosition));
}

void Entities::sortEntitiesByDrawOrder(Container& entities)
{
    auto drawOrderSorting = [](const Entities::Pointer& entityLhs, const Entities::Pointer& entityRhs)
                            {
                                    return (entityLhs->getDrawOrder() < entityRhs->getDrawOrder());
                            };

    std::stable_sort(entities.begin(), entities.end(), drawOrderSorting);
}
