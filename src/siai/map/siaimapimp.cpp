#include "map/siaimapimp.hpp"
#include "map/mapentity.hpp"
#include "map/commandimp.hpp"

#include "util/reversion.hpp"

#include <algorithm>

SIAIMapImp::SIAIMapImp() = default;
SIAIMapImp::~SIAIMapImp() = default;

void SIAIMapImp::testDrawAll(Painter& painter)
{
    auto command = MapCommand::create("draw-all", m_entities, painter);
    command->execute();
}

int SIAIMapImp::getNumberOfColumns() const noexcept
{
    return m_numberOfColumns;
}

int SIAIMapImp::getNumberOfRows() const noexcept
{
    return m_numberOfRows;
}

void SIAIMapImp::reset(int numberOfColumns, int numberOfRows)
{
    m_numberOfColumns = numberOfColumns;
    m_numberOfRows = numberOfRows;

    m_entities.resize(0);

    generateCells();
}

void SIAIMapImp::repaint(Painter& painter)
{
    for(const auto& entity : m_entities)
    {
        entity->draw(painter);
    }
}

void SIAIMapImp::selectEntity(const PanelPoint& point)
{
    for(const auto& entity : reverse(m_entities))
    {
        bool someEntityChanged = selectOrDiselectIfHasPointInside(entity.get(), point);

        if(someEntityChanged) return;
    }
}

void SIAIMapImp::diselectAllEntities()
{
    for(const auto& entity : m_entities)
    {
        entity->diselect();
    }
}

MapPosition SIAIMapImp::getLastSelectedPosition() const noexcept
{
    MapPosition lastSelectedPosition{0, 0};

    for(const auto& entity : reverse(m_entities))
    {
        if(entity->isSelected())
            lastSelectedPosition = entity->getPosition();
    }

    return lastSelectedPosition;
}

int SIAIMapImp::getLastSelectedId() const noexcept
{
    int lastSelectedId{0};

    for(const auto& entity : m_entities)
    {
        if(entity->isSelected())
            lastSelectedId = entity->getId();
    }

    return lastSelectedId;
}

void SIAIMapImp::replaceCell(const std::string& type, const PanelPoint& point)
{
    EntityIterator cellIteratorFromEntitiesVector = findCellIteratorWithPoint(point);

    if(cellIteratorFromEntitiesVector == m_entities.end())
    {
        return;
    }

    const MapPosition& cellFoundPosition = (*cellIteratorFromEntitiesVector)->getPosition();
    bool cellOccupied = isCellOccupied(cellFoundPosition);

    if(cellOccupied)
    {
        return;
    }

    createCopyWithDifferentTypeAndEraseOriginal(cellIteratorFromEntitiesVector, type);

    sortEntitiesByDrawOrder();
}

EntityIterator SIAIMapImp::findCellIteratorWithPoint(const PanelPoint& point)
{
    auto findCellWithPointInside = [&point](const EntityPointer& entity)
                                    {
                                        bool entityIsACell = dynamic_cast<ICell*>(entity.get()) != nullptr;

                                        return (entity->hasPointInside(point)
                                                && entityIsACell);
                                    };

    return std::find_if(m_entities.begin(), m_entities.end(), findCellWithPointInside);
}

bool SIAIMapImp::isCellOccupied(const MapPosition& position) const
{
    auto findADifferentEntityWithPosition = [&position](const EntityPointer& entity)
                                            {
                                                bool entityIsNotACell = dynamic_cast<ICell*>(entity.get()) == nullptr;

                                                return (entity->getPosition().column == position.column
                                                        && entity->getPosition().row == position.row
                                                        && entityIsNotACell);
                                            };

    auto entityIterator = std::find_if(m_entities.begin(), m_entities.end(), findADifferentEntityWithPosition);

    if(entityIterator == m_entities.end())
    {
        return false;
    }

    return true;
}

bool SIAIMapImp::selectOrDiselectIfHasPointInside(IMapEntity* entity, const PanelPoint& point) noexcept
{
    if(entity->hasPointInside(point))
    {
        if(!entity->isSelected())
            entity->select();
        else
            entity->diselect();
        return true;
    }

    return false;
}

void SIAIMapImp::createCopyWithDifferentTypeAndEraseOriginal(EntityIterator& entityIterator, const std::string& type)
{
    int newId = (*entityIterator)->getId();
    MapPosition newPosition = (*entityIterator)->getPosition();

    m_entities.erase(entityIterator);
    m_entities.push_back(ICell::create( type, newId, newPosition ));
}

void SIAIMapImp::sortEntitiesByDrawOrder()
{
    auto drawOrderSorting = [](const EntityPointer& entityLhs, const EntityPointer& entityRhs)
                            {
                                    return (entityLhs->getDrawOrder() < entityRhs->getDrawOrder());
                            };

    std::stable_sort(m_entities.begin(), m_entities.end(), drawOrderSorting);
}

void SIAIMapImp::generateCells()
{
    int idGenerator{0};

    for(int column = 0; column < m_numberOfColumns; ++column)
    {
        for(int row = 0; row < m_numberOfRows; ++row)
        {
            int id = ++idGenerator;
            MapPosition position{column, row, MapDirection::RIGHT};

            EntityPointer tmpCell = ICell::create("Regular", id, position);

            m_entities.push_back(std::move(tmpCell));

            ///////////////////////////TEST//////////////////////////
            if(position.column < 20)
            {
                EntityPointer tmpAgv = IAgv::create("Regular", id, position);
                m_entities.push_back(std::move(tmpAgv));
            }

        }
    }
}
