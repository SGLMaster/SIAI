#include "map/cell.hpp"
#include "map/cellimp.hpp"

#include <wx/dcclient.h>

std::unique_ptr<Cell> Cell::createRegularCell(int column, int row)
{
    return std::make_unique<RegularCell>(column, row);
}

std::unique_ptr<Cell> Cell::createBlockedCell(int column, int row)
{
    return std::make_unique<BlockedCell>(column, row);
}

Cell::Cell() = default;
Cell::Cell(int column, int row) : MapEntity(column, row) {}
Cell::~Cell() = default;

void Cell::draw(PainterContainer& painter)
{
    calculateZoomedSideLength(painter.zoom);
    calculateOrigin();

    if(isVisibleOnScreen(painter.panelOrigin, painter.panelSize))
    {
        if(!m_selected)
            painter.drawClient.SetPen(*wxBLACK_PEN);
        else
            painter.drawClient.SetPen(*wxRED_PEN);

        painter.drawClient.DrawRectangle(m_origin.x, m_origin.y, m_sideLength, m_sideLength);
    }
}

void Cell::setPosition(int column, int row)
{
    m_column = column;
    m_row = row;

    int defaultZoom{1};
    calculateZoomedSideLength(defaultZoom);
    calculateOrigin();
}

void Cell::setSelected(const PanelPoint& mousePosition)
{
    m_selected = true;
}

void Cell::diselect() noexcept
{
    m_selected = false;
}

bool Cell::isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept
{
    int panelLimitX = panelOrigin.x + panelSize.width;
    int panelLimitY = panelOrigin.y + panelSize.height;

    int minAllowedCellOriginX = panelOrigin.x - m_sideLength;
    int minAllowedCellOriginY = panelOrigin.y - m_sideLength;

    if(m_origin.x < panelLimitX && m_origin.y < panelLimitY
       && m_origin.x >= minAllowedCellOriginX && m_origin.y >= minAllowedCellOriginY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Cell::isUnderMouse(const PanelPoint& mousePosition) const noexcept
{
    if(mousePosition.x > m_origin.x && mousePosition.y > m_origin.y
        && mousePosition.x <= ( m_origin.x + m_sideLength ) && mousePosition.y <= ( m_origin.y + m_sideLength ))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Cell::calculateZoomedSideLength(int zoom)
{
    m_sideLength = SIAIGlobals::CELLS_DEFAULT_WIDTH_PX*zoom;
}

void Cell::calculateOrigin()
{
    m_origin = PanelPoint{m_column*m_sideLength, m_row*m_sideLength};
}
