#include "map/siaimapimp.hpp"
#include "map/mapentity.hpp"

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

    m_cells.resize(0);

    fillEntitiesVectorWithCells();
}

void SIAIMapImp::repaint(Painter& painter)
{
    for(const auto& cell : m_cells)
    {
        cell->draw(painter);
    }
}

void SIAIMapImp::selectEntityWithPoint(const PanelPoint& point)
{
    for(const auto& cell : m_cells)
    {
        if(cell->hasPointInside(point))
            cell->setSelected();
    }
}

void SIAIMapImp::diselectAllEntities()
{
    for(const auto& entity : m_cells)
    {
        entity->diselect();
    }
}

void SIAIMapImp::replaceCell(const std::string& type, const PanelPoint& position)
{
    for(std::vector<CellPointer>::const_iterator iter = m_cells.begin(); iter != m_cells.end(); ++iter)
    {
        if((*iter)->hasPointInside(position))
        {
            int newCellColumn = (*iter)->getColumn();
            int newCellRow = (*iter)->getRow();

            m_cells.erase(iter);

            m_cells.push_back(ICell::create(type, newCellColumn, newCellRow));
            break;
        }
    }
}

void SIAIMapImp::eraseEntityWithPoint(const PanelPoint& point)
{
    for(std::vector<CellPointer>::const_iterator iter = m_cells.begin(); iter != m_cells.end(); ++iter)
    {
        if((*iter)->hasPointInside(point))
        {
            m_cells.erase(iter);
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
            m_cells.push_back(ICell::create("Regular", column, row));
        }
    }
}
