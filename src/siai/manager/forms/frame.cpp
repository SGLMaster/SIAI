#include "database/database.hpp"

#include "manager/forms/frame.hpp"
#include "manager/forms/newmapdialog.hpp"
#include "manager/forms/loadmapdialog.hpp"
#include "manager/forms/dbsettingsdialog.hpp"
#include "manager/threads.hpp"

#include "painter/painter.hpp"

#include "util/database.hpp"
#include "util/string.hpp"
#include "util/map.hpp"

#include "map/siaimap.hpp"

#include "globals.hpp"
#include "log.hpp"

#include "manager/app.hpp"

#include <wx/dcclient.h>
#include <wx/utils.h>

ManagerFrame::ManagerFrame(wxWindow* parent) : Forms::ManagerFrame(parent), m_mapControl{NULL}
{
    m_scrolledMapPanel->SetDoubleBuffered(true);

    wxImage::AddHandler(new wxPNGHandler);

    m_originalFrameTitle = GetTitle();

    updateFrameTitle();
    updateToolbar();
}

 ManagerFrame::~ManagerFrame()
 {
    {
        wxCriticalSectionLocker locker(wxGetApp().m_criticalSection);

        const wxArrayThread& threads = wxGetApp().m_threads;
        size_t numberOfThreadsRunning = threads.GetCount();

        if(numberOfThreadsRunning == 0)
            return;

        wxGetApp().m_shuttingDown = true;
    }

    // We wait for the threads to terminate
    wxGetApp().m_semaphoreAllDone.Wait();
 }

void ManagerFrame::initializeNewMap(int numberOfColumns, int numberOfRows, const std::string& mapName)
{
    if(!m_mapControl)
        m_mapControl = SIAIMap::createMap();

    m_mapControl->setName(mapName);
    m_mapControl->reset(numberOfColumns, numberOfRows);
    m_mapControl->createDatabaseTables(*m_dbConnector);

    m_editingEnabled = true;

    updateFrameTitle();
    updateToolbar();
    repaintMapNow();
    updateScrollbarsSize();
}

void ManagerFrame::loadMap(const std::string& mapName)
{
    if(!m_mapControl)
        m_mapControl = SIAIMap::createMap();

    m_mapControl->setName(mapName);
	m_mapControl->loadFromDb(*m_dbConnector);

    m_timerRefreshMap.Start();

    m_editingEnabled = true;

    updateFrameTitle();
    updateToolbar();
    repaintMapNow();
    updateScrollbarsSize();
}

void ManagerFrame::OnClosing(wxCloseEvent& event)
{
    //We stop the timer that repaints the map so the app can stop
    m_timerRefreshMap.Stop();
	    
    Forms::ManagerFrame::Close();
}

void ManagerFrame::OnLeftClickMapPanel(wxMouseEvent& event)
{
	try
	{
		assertDatabaseConnected();
	}
	catch(const std::exception& e)
	{
		Log::warning(e.what());
		return;
	}

    callCurrentToolAction();

    updateFrameTitle();
    updateStatusBar();
    repaintMapNow();
}

void ManagerFrame::OnSelectionNewMap(wxCommandEvent& event)
{
    if(isUpdateMapThreadRunningMessages())
		return;

    if(!isDbConnectedMessages())
        return;

    NewMapDialog* newMapDialog = new NewMapDialog(this);
    newMapDialog->Show();
    Disable();
}

void ManagerFrame::OnSelectionLoadMap(wxCommandEvent& event)
{
    if(isUpdateMapThreadRunningMessages())
		return;

    if(!isDbConnectedMessages())
        return;

    resetUpdateMapThread();

	LoadMapDialog* loadMapDialog = new LoadMapDialog(this);
    loadMapDialog->loadMapsListFromDb(*m_dbConnector);
	loadMapDialog->Show();
	Disable();
}

void ManagerFrame::OnSelectionDbSettings(wxCommandEvent& event)
{
	DbSettingsDialog* dbSettingsDialog = new DbSettingsDialog(this);
	dbSettingsDialog->Show();
	Disable();
}

void ManagerFrame::OnToolSelect(wxCommandEvent& event)
{
    m_currentTool = Tool::SELECT;
}

void ManagerFrame::OnToolRegularCell(wxCommandEvent& event)
{
    m_currentTool = Tool::REGULAR_CELL;
}

void ManagerFrame::OnToolBlockedCell(wxCommandEvent& event)
{
    m_currentTool = Tool::BLOCKED_CELL;
}

void ManagerFrame::OnToolStorageCell(wxCommandEvent& event)
{
    m_currentTool = Tool::STORAGE_CELL;
}

void ManagerFrame::OnToolIngressCell(wxCommandEvent& event)
{
    m_currentTool = Tool::INGRESS_CELL;
}

void ManagerFrame::OnToolParkingCell(wxCommandEvent& event)
{
    m_currentTool = Tool::PARKING_CELL;
}

void ManagerFrame::OnToolAddAgv(wxCommandEvent& event)
{
    m_currentTool = Tool::ADD_AGV;
}

void ManagerFrame::OnToolAddRack(wxCommandEvent& event)
{
    m_currentTool = Tool::ADD_RACK;
}


void ManagerFrame::OnToolTurnLeft(wxCommandEvent& event)
{
    m_currentTool = Tool::TURN_ENTITY_LEFT;
}

void ManagerFrame::OnToolTurnRight(wxCommandEvent& event)
{
	m_currentTool = Tool::TURN_ENTITY_RIGHT;
}

void ManagerFrame::OnToolUndo(wxCommandEvent& event)
{
    m_mapControl->undo(*m_dbConnector);

    repaintMapNow();
}

void ManagerFrame::OnToolRedo(wxCommandEvent& event)
{
    m_mapControl->redo(*m_dbConnector);

    repaintMapNow();
}

void ManagerFrame::OnToolConnectDb(wxCommandEvent& event)
{
    m_dbConnectionOptions = Util::Db::loadDbOptionsFromFile(SIAIGlobals::DB_CONFIG_FILENAME);
    m_dbConnectionOptions.schema = SIAIGlobals::DB_NAME;

	tryToConnectToDatabase();

    updateToolbar();
    updateFrameTitle();
}

void ManagerFrame::OnToolDisconnectDb(wxCommandEvent& event)
{
    resetUpdateMapThread();

    m_dbConnector.reset();

    updateToolbar();
    updateFrameTitle();
}

void ManagerFrame::OnToolPlay(wxCommandEvent& event)
{
    if(isDbConnected() && m_mapControl)
    {
        createAndRunUpdateMapThread();

        m_editingEnabled = false;
        updateToolbar();
    }
}

void ManagerFrame::OnToolStop(wxCommandEvent& event)
{
    resetUpdateMapThread();

    m_editingEnabled = true;
    updateToolbar();
}

void ManagerFrame::OnSliderZoom(wxCommandEvent& event)
{
    m_mapPanelZoom = m_sliderZoom->GetValue();

    repaintMapNow();
    updateScrollbarsSize();
}

void ManagerFrame::OnEnterMapPanel( wxMouseEvent& event )
{

}

void ManagerFrame::OnLeaveMapPanel( wxMouseEvent& event )
{

}

void ManagerFrame::OnPaintMapPanel( wxPaintEvent& event )
{
    wxPaintDC paintDC(m_scrolledMapPanel);
    prepareDCAndPaintMap(paintDC);
}

void ManagerFrame::OnTimerRefreshMap(wxTimerEvent& event)
{
    if(m_mapControl && isDbConnected())
    {
        repaintMapNow();
    }
}

bool ManagerFrame::isUpdateMapThreadRunningMessages()
{
    std::string isRunningMsg{"Debes detener la supervision para poder usar esta opcion!"};

    if(isUpdateMapThreadRunning())
	{
		Log::warning(isRunningMsg);
		return true;
	}

    return false;
}

bool ManagerFrame::isUpdateMapThreadRunning()
{
    if(wxGetApp().m_updateMapThread == NULL)
        return false;

    return true;
}

void ManagerFrame::createAndRunUpdateMapThread()
{
    // If the thread doesn't exist already
    if(wxGetApp().m_updateMapThread == NULL)
    {
        ManagerThread* thread = new UpdateMapThread(m_mapControl, m_dbConnector.get());

        if (thread->Create() != wxTHREAD_NO_ERROR)
        {
            Log::error("No se puede crear el thread!");
            exit(-1);
        }

        wxCriticalSectionLocker enter(wxGetApp().m_criticalSection);
        wxGetApp().m_updateMapThread = thread;
        wxGetApp().m_threads.Add(wxGetApp().m_updateMapThread);

        if(thread->Run() != wxTHREAD_NO_ERROR)
        {
            Log::error("No se puede iniciar el thread!");
            exit(-1);
        }
    }
}

void ManagerFrame::resetUpdateMapThread()
{
    // If there's already one UpdateMapThread created running we destroy it and reset
    // the pointer so we can start a new one
    if(wxGetApp().m_updateMapThread != NULL)
    {
        wxGetApp().m_updateMapThread->Delete();

        wxGetApp().m_updateMapThread = NULL;
    }
}

bool ManagerFrame::isDbConnectedMessages()
{
    std::string disconnectedMsg{"Base de datos desconectada!"};

    if(!isDbConnected())
	{
		Log::warning(disconnectedMsg);
		return false;
	}

    return true;
}

bool ManagerFrame::isDbConnected()
{
    if(!m_dbConnector)
	{
		return false;
	}
    else
    {
        if(!m_dbConnector->isConnected())
		    return false;
    }

    return true;
}

void ManagerFrame::tryToConnectToDatabase()
{
	try
	{
		auto connector = DbConnector::makeConnector(m_dbConnectionOptions);

		m_dbConnector = std::move(connector);
	}
	catch(const DbConnectionException& e)
	{
		Log::warning(std::string("Error al conectar a base de datos: ") + e.what());

		m_dbConnector.reset(nullptr);
	}

    updateFrameTitle();
}

void ManagerFrame::assertDatabaseConnected()
{
	if(!m_dbConnector)
	{
		throw DbNotConnectedException();
	}
	else
	{
		if(!m_dbConnector->isConnected())
		{
			throw DbNotConnectedException();
		}
	}
}

void ManagerFrame::callCurrentToolAction()
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
        actionToolReplaceCell("Regular", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::BLOCKED_CELL:
        actionToolReplaceCell("Blocked", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::STORAGE_CELL:
        actionToolReplaceCell("Storage", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::INGRESS_CELL:
        actionToolReplaceCell("Ingress", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::PARKING_CELL:
        actionToolReplaceCell("Parking", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::ADD_AGV:
        actionToolAddAgv("RegularAgv", columnToPerformAction, rowToPerformAction);
        break;
    case Tool::ADD_RACK:
        actionToolAddRack("RegularRack", columnToPerformAction, rowToPerformAction);
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

PanelPoint ManagerFrame::getMousePositionRelativeToMapPanelOrigin()
{
    PanelPoint mousePos{wxGetMousePosition().x, wxGetMousePosition().y};

    int mouseX = mousePos.x - m_scrolledMapPanel->GetScreenPosition().x;
    int mouseY = mousePos.y - m_scrolledMapPanel->GetScreenPosition().y;

    wxPoint mouseRelativePos( m_scrolledMapPanel->CalcUnscrolledPosition( wxPoint(mouseX, mouseY) ) );

    return PanelPoint{mouseRelativePos.x, mouseRelativePos.y};
}

void ManagerFrame::actionToolSelect(PanelPoint& mousePosition)
{
    if(!wxGetKeyState(WXK_CONTROL))
    {
        m_mapControl->diselectAll();
    }

    m_mapControl->selectEntity(mousePosition);
}

void ManagerFrame::actionToolReplaceCell(const std::string& cellType, int column, int row)
{
    std::string commandReplaceCell = Util::String::generateCommand("replace-cell", cellType, m_mapControl->getName(),
    		column, row);

    m_mapControl->executeCommand(commandReplaceCell, *m_dbConnector);
}

void ManagerFrame::actionToolAddAgv(const std::string& agvType, int column, int row)
{
    std::string commandAddAgv = Util::String::generateCommand("add-agv", agvType, m_mapControl->getName(), 
                                                                column, row);

    m_mapControl->executeCommand(commandAddAgv, *m_dbConnector);
}

void ManagerFrame::actionToolAddRack(const std::string& rackType, int column, int row)
{
    std::string commandAddRack = Util::String::generateCommand("add-rack", rackType, m_mapControl->getName(), 
                                                                column, row);

    m_mapControl->executeCommand(commandAddRack, *m_dbConnector);
}

void ManagerFrame::actionToolTurn(const std::string& direction, int column, int row)
{
	std::string commandTurn = Util::String::generateCommand("turn-entity", direction, m_mapControl->getName(),
			column, row);

	m_mapControl->executeCommand(commandTurn, *m_dbConnector);
}

void ManagerFrame::repaintMapNow()
{
    wxClientDC clientDC(m_scrolledMapPanel);
    prepareDCAndPaintMap(clientDC);
}

void ManagerFrame::prepareDCAndPaintMap(wxDC &dc)
{
    m_scrolledMapPanel->DoPrepareDC(dc);

    auto painter = Painter::createWxPainter(dc, calculatePainterData());

    if(m_mapControl)
        m_mapControl->repaint(*painter);
}

PanelData ManagerFrame::calculatePainterData() const
{
    wxPoint panelOrigin = m_scrolledMapPanel->CalcUnscrolledPosition( wxPoint(0, 0) );
    wxSize panelSize = m_scrolledMapPanel->GetSize();

    PanelPoint painterOrigin{panelOrigin.x, panelOrigin.y};
    PanelSize painterSize{panelSize.GetWidth(), panelSize.GetHeight()};

    return PanelData{painterOrigin, painterSize, m_mapPanelZoom};
}

void ManagerFrame::updateFrameTitle()
{
    std::string newFrameTitle;

    if(m_mapControl)
    {   
        if(m_mapControl->getName() != "")
            newFrameTitle += m_mapControl->getName() + " - " + m_originalFrameTitle;
        else
            newFrameTitle = m_originalFrameTitle;
    }
    else
        newFrameTitle = m_originalFrameTitle;

    if(isDbConnected())
        newFrameTitle += " [Conectado]";
    else
        newFrameTitle += " [Desconectado]";

    SetTitle(newFrameTitle);
}

void ManagerFrame::updateToolbar()
{
    if(m_editingEnabled)
    {
        m_toolBar1->EnableTool(m_toolRegularCell->GetId(), true);
        m_toolBar1->EnableTool(m_toolBlockedCell->GetId(), true);
        m_toolBar1->EnableTool(m_toolStorageCell->GetId(), true);
        m_toolBar1->EnableTool(m_toolIngressCell->GetId(), true);
        m_toolBar1->EnableTool(m_toolParkingCell->GetId(), true);
        m_toolBar1->EnableTool(m_toolAddAgv->GetId(), true);
        m_toolBar1->EnableTool(m_toolAddRack->GetId(), true);
        m_toolBar1->EnableTool(m_toolTurnLeft->GetId(), true);
        m_toolBar1->EnableTool(m_toolTurnRight->GetId(), true);
        m_toolBar1->EnableTool(m_toolUndo->GetId(), true);
        m_toolBar1->EnableTool(m_toolRedo->GetId(), true);
    }
    else
    {
        m_toolBar1->EnableTool(m_toolRegularCell->GetId(), false);
        m_toolBar1->EnableTool(m_toolBlockedCell->GetId(), false);
        m_toolBar1->EnableTool(m_toolStorageCell->GetId(), false);
        m_toolBar1->EnableTool(m_toolIngressCell->GetId(), false);
        m_toolBar1->EnableTool(m_toolParkingCell->GetId(), false);
        m_toolBar1->EnableTool(m_toolAddAgv->GetId(), false);
        m_toolBar1->EnableTool(m_toolAddRack->GetId(), false);
        m_toolBar1->EnableTool(m_toolTurnLeft->GetId(), false);
        m_toolBar1->EnableTool(m_toolTurnRight->GetId(), false);
        m_toolBar1->EnableTool(m_toolUndo->GetId(), false);
        m_toolBar1->EnableTool(m_toolRedo->GetId(), false);
    }

    if(wxGetApp().m_updateMapThread == NULL && isDbConnected() && m_mapControl)
    {
        m_toolBar1->EnableTool(m_toolPlay->GetId(), true);
        m_toolBar1->EnableTool(m_toolStop->GetId(), false);
    }
    else if(wxGetApp().m_updateMapThread != NULL)
    {
        m_toolBar1->EnableTool(m_toolPlay->GetId(), false);
        m_toolBar1->EnableTool(m_toolStop->GetId(), true);
    }
    else
    {
        m_toolBar1->EnableTool(m_toolPlay->GetId(), false);
        m_toolBar1->EnableTool(m_toolStop->GetId(), false);
    }

    if(isDbConnected())
    {
        m_toolBar1->EnableTool(m_toolConnectDb->GetId(), false);
        m_toolBar1->EnableTool(m_toolDisconnectDb->GetId(), true);
    }
    else
    {
        m_toolBar1->EnableTool(m_toolConnectDb->GetId(), true);
        m_toolBar1->EnableTool(m_toolDisconnectDb->GetId(), false);
    }
}

void ManagerFrame::updateStatusBar()
{
    updateSelectedIdOnStatusBar();
    updateSelectedPositionOnStatusBar();
}

void ManagerFrame::updateSelectedIdOnStatusBar()
{
    int lastSelectedId{m_mapControl->getSelectedId()};

    wxString lastSelectedIdString;
    lastSelectedIdString.Printf("%.10d", lastSelectedId);

    m_statusBar->SetStatusText(lastSelectedIdString, static_cast<int>(StatusBarFields::SELECTED_ID));
}

void ManagerFrame::updateSelectedPositionOnStatusBar()
{
    int selectedEntityColumn = m_mapControl->getSelectedEntityColumn();
    int selectedEntityRow = m_mapControl->getSelectedEntityRow();

    wxString lastSelectedPositionString;
    lastSelectedPositionString << _("Columna: ") << selectedEntityColumn << _(", Fila: ") << selectedEntityRow;

    m_statusBar->SetStatusText(lastSelectedPositionString, static_cast<int>(StatusBarFields::SELECTED_POSITION));
}

void ManagerFrame::updateScrollbarsSize()
{
    int numberOfColsInMap = 0;
    int numberOfRowsInMap = 0;

    if(m_mapControl)
    {
        numberOfColsInMap = m_mapControl->getNumberOfColumns();
        numberOfRowsInMap = m_mapControl->getNumberOfRows();
    }

    int mapCellsWidth = SIAIGlobals::CELLS_DEFAULT_WIDTH_PX;

    int virtualScrollingWidth = numberOfColsInMap*mapCellsWidth*m_mapPanelZoom;
    int virtualScrollingHeight = numberOfRowsInMap*mapCellsWidth*m_mapPanelZoom;

    m_scrolledMapPanel->SetVirtualSize(virtualScrollingWidth, virtualScrollingHeight);
}

