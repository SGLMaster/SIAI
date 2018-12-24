#pragma once

#include <mapeditor.h>

class SIAIMap;
class PanelPoint;
class PanelData;

class wxDC;

enum class Tool
{
    TOOL_SELECT,
    TOOL_REGULAR_CELL,
    TOOL_BLOCKED_CELL
};

class MapEditorFrame : public Forms::MapEditorFrame
{
private:
    SIAIMap* m_mapControl;

    Tool m_currentTool;

    int m_mapPanelZoom{1};

public:
    MapEditorFrame(wxWindow* parent);

    void initializeNewMap(int numberOfColumns, int numberOfRows);

private:
    virtual void OnLeftClickMapPanel(wxMouseEvent& event) override;

    virtual void OnSelectionNewMap(wxCommandEvent& event) override;

    virtual void OnToolSelect(wxCommandEvent& event) override;
    virtual void OnToolRegularCell( wxCommandEvent& event ) override;
	virtual void OnToolBlockedCell( wxCommandEvent& event ) override;

    virtual void OnPaintMapPanel( wxPaintEvent& event ) override;

    void actionToolSelect(PanelPoint& mousePosition);

    void repaintMapNow();
    void prepareDCAndPaintMap(wxDC& dc);
    PanelData calculatePainterData() const;

    void updateScrollbarsSize();

    PanelPoint getMousePositionRelativeToMapPanelOrigin();
};
