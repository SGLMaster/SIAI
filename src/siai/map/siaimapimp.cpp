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
    m_cells.resize(0);
    m_agvs.resize(0);

    fillCellsVector();
}

void SIAIMapImp::repaint(Painter& painter)
{
    for(const auto& entity : m_entities)
    {
        entity->draw(painter);
    }
}

void SIAIMapImp::selectEntityWithPoint(const PanelPoint& point)
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

void SIAIMapImp::replaceCell(const std::string& type, const PanelPoint& position)
{
    auto cellPointerInCellsVector = std::find_if(m_cells.begin(), m_cells.end(),
                                                 [&position](const CellPointer& cell)
                                                 { return cell->hasPointInside(position); });

    if(cellPointerInCellsVector == m_cells.end())
        return;

    int matchCellId = (*cellPointerInCellsVector)->getId();

    auto cellPointerInEntitiesVector = std::find_if(m_entities.begin(), m_entities.end(),
                                                    [&matchCellId](const EntityPointer& entity)
                                                    {
                                                        return (entity->getId() == matchCellId
                                                                && dynamic_cast<ICell*>(entity.get()) );
                                                    });

    if(cellPointerInEntitiesVector == m_entities.end())
        return;

    auto cellToCopyData = cellPointerInCellsVector;

    int newId = (*cellToCopyData)->getId();
    MapPosition newPosition = (*cellToCopyData)->getPosition();

    m_cells.erase(cellPointerInCellsVector);
    m_entities.erase(cellPointerInEntitiesVector);

    m_cells.push_back(ICell::create( type, newId, newPosition ));
    m_entities.push_back( m_cells.back() );
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

void SIAIMapImp::eraseEntityWithPoint(const PanelPoint& point)
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

void SIAIMapImp::fillCellsVector()
{
    int idGenerator{0};

    for(int column = 0; column < m_numberOfColumns; ++column)
    {
        for(int row = 0; row < m_numberOfRows; ++row)
        {
            CellPointer tmpCell = ICell::create( "Regular", ++idGenerator, MapPosition{column, row, MapDirection::RIGHT} );
            AgvPointer tmpAgv = IAgv::create( "Regular", ++idGenerator, MapPosition{column, row, MapDirection::RIGHT} );

            m_cells.push_back(std::move(tmpCell));
            m_agvs.push_back(std::move(tmpAgv));

            m_entities.push_back(m_cells.back());
            m_entities.push_back(m_agvs.back());
        }
    }
}
