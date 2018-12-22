#include "map/siaimapimp.hpp"
#include "map/mapentity.hpp"
#include "map/cell.hpp"

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

    fillEntitiesVectorWithCells();
}

void SIAIMapImp::repaint(PainterContainer& painter)
{
    for(const auto& entity : m_entities)
    {
        entity->draw(painter);
    }
}

void SIAIMapImp::selectEntityUnderMouse(const PanelPoint& mousePosition)
{
    for(const auto& entity : m_entities)
    {
        if(entity->isUnderMouse(mousePosition))
            entity->setSelected(mousePosition);
    }
}

void SIAIMapImp::diselectAllEntities()
{
    for(const auto& entity : m_entities)
    {
        entity->diselect();
    }
}

void SIAIMapImp::placeBlockedCell(const PanelPoint& position)
{
    for(std::vector<EntityPointer>::const_iterator iter = m_entities.begin(); iter != m_entities.end(); ++iter)
    {
        if((*iter)->isUnderMouse(position))
        {
            int newCellColumn = (*iter)->getColumn();
            int newCellRow = (*iter)->getRow();

            m_entities.erase(iter);

            m_entities.push_back(Cell::createBlockedCell(newCellColumn, newCellRow));
            break;
        }
    }
}

void SIAIMapImp::eraseEntityUnderMouse(const PanelPoint& position)
{
    for(std::vector<EntityPointer>::const_iterator iter = m_entities.begin(); iter != m_entities.end(); ++iter)
    {
        if((*iter)->isUnderMouse(position))
        {
            m_entities.erase(iter);
            break;
        }
    }
}

void SIAIMapImp::fillEntitiesVectorWithCells()
{
    for(int column = 0; column < m_numberOfColumns; ++column)
    {
        for(int row = 0; row < m_numberOfRows; ++row)
        {
            m_entities.push_back(Cell::createRegularCell(column, row));
        }
    }
}
