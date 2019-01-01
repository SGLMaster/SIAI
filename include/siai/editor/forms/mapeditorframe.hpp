#pragma once

#include <editor/forms/mapeditor.h>

#include <string>

class SIAIMap;
class PanelPoint;
class PanelData;

class wxDC;

enum class Tool
{
    SELECT,
    REGULAR_CELL,
    BLOCKED_CELL,
    ADD_AGV
};

enum class StatusBarFields
{
    TOOL,
    SELECTED_ID,
    SELECTED_POSITION
};

class MapEditorFrame : public Forms::MapEditorFrame
{
private:
    SIAIMap* m_mapControl;

    Tool m_currentTool{Tool::SELECT};

    int m_mapPanelZoom{1};

public:
    MapEditorFrame(wxWindow* parent);

    void initializeNewMap(int numberOfColumns, int numberOfRows);

private:
    virtual void OnLeftClickMapPanel(wxMouseEvent& event) override;

    virtual void OnSelectionNewMap(wxCommandEvent& event) override;

    virtual void OnToolSelect(wxCommandEvent& event) override;
    virtual void OnToolRegularCell(wxCommandEvent& event) override;
	virtual void OnToolBlockedCell(wxCommandEvent& event) override;
	virtual void OnToolAddAgv(wxCommandEvent& event) override;
	virtual void OnToolUndo(wxCommandEvent& event) override;
	virtual void OnToolRedo(wxCommandEvent& event) override;
	virtual void OnToolTurnLeft(wxCommandEvent& event) override;
    virtual void OnToolTurnRight(wxCommandEvent& event) override;

	virtual void OnSliderZoom(wxCommandEvent& event) override;

    virtual void OnPaintMapPanel(wxPaintEvent& event) override;

    void callCurrentToolAction();
    void actionToolSelect(PanelPoint& mousePosition);
    void actionToolReplaceCell(const std::string& cellType, const PanelPoint& mousePosition);
    void actionToolAddAgv(const std::string& agvType, const PanelPoint& mousePosition);

    void repaintMapNow();
    void prepareDCAndPaintMap(wxDC& dc);
    PanelData calculatePainterData() const;

    void updateScrollbarsSize();
    void updateStatusBar();
    void updateSelectedIdOnStatusBar();
    void updateSelectedPositionOnStatusBar();

    PanelPoint getMousePositionRelativeToMapPanelOrigin();
};
