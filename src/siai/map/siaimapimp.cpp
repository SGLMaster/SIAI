#include "map/siaimapimp.hpp"
#include "map/mapentity.hpp"

#include "util/reversion.hpp"

#include <algorithm>

SIAIMapImp::SIAIMapImp() = default;
SIAIMapImp::~SIAIMapImp() = default;

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

EntityIterator SIAIMapImp::findCellIteratorWithPoint(const PanelPoint& point)
{
    auto findCellWithPointInside = [&point](const EntityPointer& entity)
                                    {
                                        return (entity->hasPointInside(point)
                                                && dynamic_cast<ICell*>(entity.get()) != nullptr );
                                    };

    return std::find_if(m_entities.begin(), m_entities.end(), findCellWithPointInside);
}

void SIAIMapImp::replaceCell(const std::string& type, const PanelPoint& point)
{
    EntityIterator cellIteratorFromEntitiesVector = findCellIteratorWithPoint(point);

    if(cellIteratorFromEntitiesVector == m_entities.end())
    {
        return;
    }

    createCopyWithDifferentTypeAndEraseOriginal(cellIteratorFromEntitiesVector, type);

    sortEntitiesByDrawOrder();
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

void SIAIMapImp::eraseEntity(const PanelPoint& point)
{
    /*
    for(std::vector<CellPointer>::const_iterator iter = m_cells.begin(); iter != m_cells.end(); ++iter)
    {
        if((*iter)->hasPointInside(point))
        {
            m_cells.erase(iter);
            break;
        }
    }
    */
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
            EntityPointer tmpAgv = IAgv::create("Regular", id, position);

            m_entities.push_back(std::move(tmpCell));
            m_entities.push_back(std::move(tmpAgv));
        }
    }
}
