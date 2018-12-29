#include "forms/mapeditorframe.hpp"
#include "forms/newmapdialog.hpp"

#include "map/mapentity.hpp"
#include "map/painter.hpp"

#include "util/string.hpp"

#include "siaimap.hpp"
#include "globals.hpp"

#include <wx/dcclient.h>
#include <wx/utils.h>

MapEditorFrame::MapEditorFrame(wxWindow* parent) : Forms::MapEditorFrame(parent)
{
    m_mapControl = SIAIMap::createMap();

    m_scrolledMapPanel->SetDoubleBuffered(true);

    wxImage::AddHandler(new wxPNGHandler);
}

void MapEditorFrame::initializeNewMap(int numberOfColumns, int numberOfRows)
{
    m_mapControl->reset(numberOfColumns, numberOfRows);

    repaintMapNow();
    updateScrollbarsSize();
}

void MapEditorFrame::OnLeftClickMapPanel(wxMouseEvent& event)
{
    callCurrentToolAction();

    updateStatusBar();
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
    m_currentTool = Tool::SELECT;
}

void MapEditorFrame::OnToolRegularCell(wxCommandEvent& event)
{
    m_currentTool = Tool::REGULAR_CELL;
}

void MapEditorFrame::OnToolBlockedCell(wxCommandEvent& event)
{
    m_currentTool = Tool::BLOCKED_CELL;
}

void MapEditorFrame::OnSliderZoom(wxCommandEvent& event)
{
    m_mapPanelZoom = m_sliderZoom->GetValue();

    repaintMapNow();
    updateScrollbarsSize();
}

void MapEditorFrame::OnPaintMapPanel( wxPaintEvent& event )
{
    wxPaintDC paintDC(m_scrolledMapPanel);
    prepareDCAndPaintMap(paintDC);
}

void MapEditorFrame::callCurrentToolAction()
{
    PanelPoint mousePosition = getMousePositionRelativeToMapPanelOrigin();

    switch(m_currentTool)
    {
    case Tool::SELECT:
        actionToolSelect(mousePosition);
        break;
    case Tool::REGULAR_CELL:
        actionToolReplaceCell("RegularCell", mousePosition);
        break;
    case Tool::BLOCKED_CELL:
        actionToolReplaceCell("BlockedCell", mousePosition);
        break;
    default:
        break;
    }
}

void MapEditorFrame::actionToolSelect(PanelPoint& mousePosition)
{
    if(!wxGetKeyState(WXK_CONTROL))
    {
        m_mapControl->executeCommand("diselect-all");
    }

    std::string commandSelect = Util::String::generateCommand("select", mousePosition.x, mousePosition.y);

    m_mapControl->executeCommand(commandSelect);
}

void MapEditorFrame::actionToolReplaceCell(const std::string& cellType, const PanelPoint& mousePosition)
{
    std::string commandReplaceCell = Util::String::generateCommand("replace-cell", cellType,
                                                                   mousePosition.x, mousePosition.y);

    m_mapControl->executeCommand(commandReplaceCell);
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

void MapEditorFrame::updateStatusBar()
{
    updateSelectedIdOnStatusBar();
    updateSelectedPositionOnStatusBar();
}

void MapEditorFrame::updateSelectedIdOnStatusBar()
{
    int lastSelectedId{m_mapControl->getLastSelectedId()};

    wxString lastSelectedIdString;
    lastSelectedIdString.Printf("%.10d", lastSelectedId);

    m_statusBar->SetStatusText(lastSelectedIdString, static_cast<int>(StatusBarFields::SELECTED_ID));
}

void MapEditorFrame::updateSelectedPositionOnStatusBar()
{
    MapPosition lastSelectedPosition{m_mapControl->getLastSelectedPosition()};

    wxString lastSelectedPositionString;
    lastSelectedPositionString << _("Columna: ") << lastSelectedPosition.column
                                << _(", Fila: ") << lastSelectedPosition.row;

    m_statusBar->SetStatusText(lastSelectedPositionString, static_cast<int>(StatusBarFields::SELECTED_POSITION));
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
