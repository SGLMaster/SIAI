#include "forms/mapeditorframe.hpp"
#include "forms/newmapdialog.hpp"

#include "map/painter.hpp"

#include "siaimap.hpp"
#include "globals.hpp"

#include <wx/dcclient.h>
#include <wx/utils.h>

MapEditorFrame::MapEditorFrame(wxWindow* parent) : Forms::MapEditorFrame(parent)
{
    m_mapControl = SIAIMap::createMap();

    m_scrolledMapPanel->SetDoubleBuffered(true);
}

void MapEditorFrame::initializeNewMap(int numberOfColumns, int numberOfRows)
{
    m_mapControl->reset(numberOfColumns, numberOfRows);

    repaintMapNow();
    updateScrollbarsSize();
}

void MapEditorFrame::OnLeftClickMapPanel(wxMouseEvent& event)
{
    PanelPoint mousePosition = getMousePositionRelativeToMapPanelOrigin();

    if(m_currentTool == Tool::TOOL_SELECT)
        actionToolSelect(mousePosition);
    else if(m_currentTool == Tool::TOOL_REGULAR_CELL)
        m_mapControl->replaceCell("Regular", mousePosition);
    else if(m_currentTool == Tool::TOOL_BLOCKED_CELL)
        m_mapControl->replaceCell("Blocked", mousePosition);

    repaintMapNow();
}

void MapEditorFrame::OnSelectionNewMap(wxCommandEvent& event)
{
    NewMapDialog* newMapDialog = new NewMapDialog(this);
    newMapDialog->Show();
    Disable();
}

void MapEditorFrame::OnToolSelect(wxCommandEvent& event)
{
    m_currentTool = Tool::TOOL_SELECT;
}

void MapEditorFrame::OnToolRegularCell(wxCommandEvent& event)
{
    m_currentTool = Tool::TOOL_REGULAR_CELL;
}

void MapEditorFrame::OnToolBlockedCell(wxCommandEvent& event)
{
    m_currentTool = Tool::TOOL_BLOCKED_CELL;
}

void MapEditorFrame::OnPaintMapPanel( wxPaintEvent& event )
{
    wxPaintDC paintDC(m_scrolledMapPanel);
    prepareDCAndPaintMap(paintDC);
}

void MapEditorFrame::actionToolSelect(PanelPoint& mousePosition)
{
    if(!wxGetKeyState(WXK_CONTROL))
        m_mapControl->diselectAllEntities();

    m_mapControl->selectEntityWithPoint(mousePosition);
}

void MapEditorFrame::repaintMapNow()
{
    wxClientDC clientDC(m_scrolledMapPanel);
    prepareDCAndPaintMap(clientDC);
}

void MapEditorFrame::prepareDCAndPaintMap(wxDC &dc)
{
    m_scrolledMapPanel->DoPrepareDC(dc);

    auto painter = Painter::createWxPainter(dc, calculatePainterData());
    m_mapControl->repaint(*painter);
}

PanelData MapEditorFrame::calculatePainterData() const
{
    wxPoint panelOrigin = m_scrolledMapPanel->CalcUnscrolledPosition( wxPoint(0, 0) );
    wxSize panelSize = m_scrolledMapPanel->GetSize();

    PanelPoint painterOrigin{panelOrigin.x, panelOrigin.y};
    PanelSize painterSize{panelSize.GetWidth(), panelSize.GetHeight()};

    return PanelData{painterOrigin, painterSize, m_mapPanelZoom};
}

void MapEditorFrame::updateScrollbarsSize()
{
    int numberOfColsInMap = m_mapControl->getNumberOfColumns();
    int numberOfRowsInMap = m_mapControl->getNumberOfRows();

    int mapCellsWidth = SIAIGlobals::CELLS_DEFAULT_WIDTH_PX;

    int virtualScrollingWidth = numberOfColsInMap*mapCellsWidth*m_mapPanelZoom;
    int virtualScrollingHeight = numberOfRowsInMap*mapCellsWidth*m_mapPanelZoom;

    m_scrolledMapPanel->SetVirtualSize(virtualScrollingWidth, virtualScrollingHeight);
}

PanelPoint MapEditorFrame::getMousePositionRelativeToMapPanelOrigin()
{
    PanelPoint mousePos{wxGetMousePosition().x, wxGetMousePosition().y};

    int mouseX = mousePos.x - m_scrolledMapPanel->GetScreenPosition().x;
    int mouseY = mousePos.y - m_scrolledMapPanel->GetScreenPosition().y;

    wxPoint mouseRelativePos( m_scrolledMapPanel->CalcUnscrolledPosition( wxPoint(mouseX, mouseY) ) );

    return PanelPoint{mouseRelativePos.x, mouseRelativePos.y};
}
