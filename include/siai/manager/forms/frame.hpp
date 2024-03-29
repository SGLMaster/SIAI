#pragma once

#include "manager/forms/manager.h"

#include "database/database.hpp"

#include <exception>
#include <memory>
#include <string>

class SIAIMap;
class PanelPoint;
class PanelData;

class wxDC;

class ManagerThread;

enum class Tool
{
    SELECT,
    REGULAR_CELL,
    BLOCKED_CELL,
    STORAGE_CELL,
    INGRESS_CELL,
    PARKING_CELL,
    ADD_AGV,
    ADD_RACK,
	TURN_ENTITY_LEFT,
	TURN_ENTITY_RIGHT
};

enum class StatusBarFields
{
    TOOL,
    SELECTED_ID,
    SELECTED_POSITION
};

class ManagerFrame : public Forms::ManagerFrame
{
private:
    std::string m_originalFrameTitle;

    SIAIMap* m_mapControl;

    bool m_editingEnabled{false};
    Tool m_currentTool{Tool::SELECT};

    int m_mapPanelZoom{1};

    DbConnectionOptions m_dbConnectionOptions;

    std::unique_ptr<DbConnector> m_dbConnector;

public:
    ManagerFrame(wxWindow* parent);
    virtual ~ManagerFrame();

    void initializeNewMap(int numberOfColumns, int numberOfRows, const std::string& mapName);
    void loadMap(const std::string& mapName);

    inline void setDbConnectionOptions(const DbConnectionOptions& dbConnectionOptions)
    {
        m_dbConnectionOptions = dbConnectionOptions;
    }

private:
    virtual void OnClosing(wxCloseEvent& event) override;

    virtual void OnLeftClickMapPanel(wxMouseEvent& event) override;

    virtual void OnSelectionNewMap(wxCommandEvent& event) override;
    virtual void OnSelectionLoadMap(wxCommandEvent& event) override;
    virtual void OnSelectionDbSettings(wxCommandEvent& event) override;
    virtual void OnSelectionShowDirections(wxCommandEvent& event) override;

    virtual void OnToolSelect(wxCommandEvent& event) override;
    virtual void OnToolRegularCell(wxCommandEvent& event) override;
	virtual void OnToolBlockedCell(wxCommandEvent& event) override;
    virtual void OnToolStorageCell(wxCommandEvent& event) override;
    virtual void OnToolIngressCell(wxCommandEvent& event) override;
    virtual void OnToolParkingCell(wxCommandEvent& event) override;

	virtual void OnToolAddAgv(wxCommandEvent& event) override;
    virtual void OnToolAddRack(wxCommandEvent& event) override;
    
	virtual void OnToolTurnLeft(wxCommandEvent& event) override;
    virtual void OnToolTurnRight(wxCommandEvent& event) override;

    virtual void OnToolUndo(wxCommandEvent& event) override;
	virtual void OnToolRedo(wxCommandEvent& event) override;

    virtual void OnToolConnectDb(wxCommandEvent& event) override;
    virtual void OnToolDisconnectDb(wxCommandEvent& event) override;

    virtual void OnToolPlay(wxCommandEvent& event) override;
    virtual void OnToolStop(wxCommandEvent& event) override;

	virtual void OnSliderZoom(wxCommandEvent& event) override;

	virtual void OnEnterMapPanel( wxMouseEvent& event ) override;
	virtual void OnLeaveMapPanel( wxMouseEvent& event ) override;

    virtual void OnPaintMapPanel(wxPaintEvent& event) override;

    virtual void OnTimerRefreshMap(wxTimerEvent& event) override;

    bool isUpdateMapThreadRunning();
    bool isUpdateMapThreadRunningMessages();

    void createAndRunUpdateMapThread();
    void resetUpdateMapThread();

    bool isDbConnectedMessages();
    bool isDbConnected();

    void tryToConnectToDatabase();
    void assertDatabaseConnected();

    void callCurrentToolAction();
    PanelPoint getMousePositionRelativeToMapPanelOrigin();

    void actionToolSelect(PanelPoint& mousePosition);
    void actionToolReplaceCell(const std::string& cellType, int column, int row);
    void actionToolAddAgv(const std::string& agvType, int column, int row);
    void actionToolAddRack(const std::string& rackType, int column, int row);
    void actionToolTurn(const std::string& direction, int column, int row);

    void repaintMapNow();
    void prepareDCAndPaintMap(wxDC& dc);
    PanelData calculatePainterData() const;

    void updateFrameTitle();
    void updateToolbar();
    void updateScrollbarsSize();
    void updateStatusBar();
    void updateSelectedIdOnStatusBar();
    void updateSelectedPositionOnStatusBar();
};

class DbNotConnectedException : public std::exception
{
public:
    virtual const char* what() const noexcept override
    {
        return "Base de Datos desconectada.";
    }
};
