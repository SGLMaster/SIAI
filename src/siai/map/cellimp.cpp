#include "map/cellimp.hpp"

#include "globals.hpp"

#include <wx/dcclient.h>

CellDefault::CellDefault() = default;
CellDefault::CellDefault(int column, int row) : ICell(column, row) {}
CellDefault::~CellDefault() = default;

void CellDefault::draw(Painter& painter)
{
    calculateZoomedSideLength(painter.getZoom());
    calculateOrigin();

    bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

    if(isVisible)
    {
        setDifferentPenIfSelected(painter);

        painter.drawRectangle(m_origin, PanelSize{m_sideLength, m_sideLength});
    }
}

void CellDefault::setPosition(int column, int row)
{
    m_column = column;
    m_row = row;

    int defaultZoom{1};
    calculateZoomedSideLength(defaultZoom);
    calculateOrigin();
}

void CellDefault::setSelected()
{
    m_selected = true;
}

void CellDefault::diselect() noexcept
{
    m_selected = false;
}

bool CellDefault::isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept
{
    int panelLimitX = panelOrigin.x + panelSize.width;
    int panelLimitY = panelOrigin.y + panelSize.height;

    int minValidCellOriginX = panelOrigin.x - m_sideLength;
    int minValidCellOriginY = panelOrigin.y - m_sideLength;

    return m_origin.x < panelLimitX && m_origin.y < panelLimitY
            && m_origin.x >= minValidCellOriginX && m_origin.y >= minValidCellOriginY;
}

bool CellDefault::hasPointInside(const PanelPoint& point) const noexcept
{
    return point.x > m_origin.x && point.y > m_origin.y && point.x <= ( m_origin.x + m_sideLength )
            && point.y <= ( m_origin.y + m_sideLength );
}

void CellDefault::calculateZoomedSideLength(int zoom)
{
    m_sideLength = SIAIGlobals::CELLS_DEFAULT_WIDTH_PX*zoom;
}

void CellDefault::calculateOrigin()
{
    m_origin = PanelPoint{m_column*m_sideLength, m_row*m_sideLength};
}

void CellDefault::setDifferentPenIfSelected(Painter& painter)
{
    if(!m_selected)
    {
        painter.setPen(PanelColor::BLACK);
    }
    else
    {
        painter.setPen(PanelColor::RED);
    }
}

RegularCell::RegularCell() = default;
RegularCell::RegularCell(int column, int row) : CellDefault(column, row) {}
RegularCell::~RegularCell() = default;

void RegularCell::draw(Painter& painter)
{
    painter.setBrush(PanelColor::WHITE);

    CellDefault::draw(painter);
}

BlockedCell::BlockedCell() = default;
BlockedCell::BlockedCell(int column, int row) : CellDefault(column, row) {}
BlockedCell::~BlockedCell() = default;

void BlockedCell::draw(Painter& painter)
{
    painter.setBrush(PanelColor::BLACK);

    CellDefault::draw(painter);
}
