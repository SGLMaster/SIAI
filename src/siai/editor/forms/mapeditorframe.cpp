#include "editor/forms/mapeditorframe.hpp"
#include "editor/forms/newmapdialog.hpp"
#include "editor/forms/dbsettingsdialog.hpp"

#include "database/database.hpp"

#include "painter/painter.hpp"

#include "util/string.hpp"
#include "util/map.hpp"

#include "map/siaimap.hpp"

#include "globals.hpp"
#include "log.hpp"

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
	/*
	DbConnectionOptions options{"test_schema", "localhost", 3306, "test_user", "easypass"};

	try
	{
		auto connector = DbConnector::makeConnector(options);
	}
	catch(const DbConnectionException& e)
	{
		Log::warning(e.what());
	}
	*/
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

void MapEditorFrame::OnSelectionDbSettings(wxCommandEvent& event)
{
	DbSettingsDialog* dbSettingsDialog = new DbSettingsDialog(this);
	dbSettingsDialog->Show();
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

void MapEditorFrame::OnToolAddAgv(wxCommandEvent& event)
{
    m_currentTool = Tool::ADD_AGV;
}

void MapEditorFrame::OnToolUndo(wxCommandEvent& event)
{
    m_mapControl->undo();

    repaintMapNow();
}

void MapEditorFrame::OnToolRedo(wxCommandEvent& event)
{
    m_mapControl->redo();

    repaintMapNow();
}

void MapEditorFrame::OnToolTurnLeft(wxCommandEvent& event)
{
    m_currentTool = Tool::TURN_ENTITY_LEFT;
}

void MapEditorFrame::OnToolTurnRight(wxCommandEvent& event)
{
	m_currentTool = Tool::TURN_ENTITY_RIGHT;
}

void MapEditorFrame::OnSliderZoom(wxCommandEvent& event)
{
    m_mapPanelZoom = m_sliderZoom->GetValue();

    repaintMapNow();
    updateScrollbarsSize();
}

void MapEditorFrame::OnEnterMapPanel( wxMouseEvent& event )
{

}

void MapEditorFrame::OnLeaveMapPanel( wxMouseEvent& event )
{

}


void MapEditorFrame::OnPaintMapPanel( wxPaintEvent& event )
{
    wxPaintDC paintDC(m_scrolledMapPanel);
    prepareDCAndPaintMap(paintDC);
}

void MapEditorFrame::callCurrentToolAction()
{
    PanelPoint mousePosition = getMousePositionRelativeToMapPanelOrigin();

	int columnToPerformAction = Util::translatePanelPointXToColumn(mousePosition.x, m_mapPanelZoom);
	int rowToPerformAction = Util::translatePanelPointYToRow(mousePosition.y, m_mapPanelZoom);

    switch(m_currentTool)
    {
    case Tool::SELECT:
        actionToolSelect(mousePosition);
        break;
    case Tool::REGULAR_CELL:
        actionToolReplaceCell("RegularCell", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::BLOCKED_CELL:
        actionToolReplaceCell("BlockedCell", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::ADD_AGV:
        actionToolAddAgv("RegularAgv", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::TURN_ENTITY_LEFT:
    	actionToolTurn("left", columnToPerformAction, rowToPerformAction);
    	break;
    case Tool::TURN_ENTITY_RIGHT:
    	actionToolTurn("right", columnToPerformAction, rowToPerformAction);
    	break;
    default:
        break;
    }
}

void MapEditorFrame::actionToolSelect(PanelPoint& mousePosition)
{
    if(!wxGetKeyState(WXK_CONTROL))
    {
        m_mapControl->diselectAll();
    }

    m_mapControl->selectEntity(mousePosition);
}

void MapEditorFrame::actionToolReplaceCell(const std::string& cellType, int column, int row)
{
    std::string commandReplaceCell = Util::String::generateCommand("replace-cell", cellType, column, row);

    m_mapControl->executeCommand(commandReplaceCell);
}

void MapEditorFrame::actionToolAddAgv(const std::string& agvType, int column, int row)
{
    std::string commandAddAgv = Util::String::generateCommand("add-agv", agvType, column, row);

    m_mapControl->executeCommand(commandAddAgv);
}

void MapEditorFrame::actionToolTurn(const std::string& direction, int column, int row)
{
	std::string commandTurn = Util::String::generateCommand("turn-entity", direction, column, row);

	m_mapControl->executeCommand(commandTurn);
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
    int lastSelectedId{m_mapControl->getSelectedId()};

    wxString lastSelectedIdString;
    lastSelectedIdString.Printf("%.10d", lastSelectedId);

    m_statusBar->SetStatusText(lastSelectedIdString, static_cast<int>(StatusBarFields::SELECTED_ID));
}

void MapEditorFrame::updateSelectedPositionOnStatusBar()
{
    int selectedEntityColumn = m_mapControl->getSelectedEntityColumn();
    int selectedEntityRow = m_mapControl->getSelectedEntityRow();

    wxString lastSelectedPositionString;
    lastSelectedPositionString << _("Columna: ") << selectedEntityColumn << _(", Fila: ") << selectedEntityRow;

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
