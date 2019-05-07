#pragma once

#include <editor/forms/mapeditor.h>

class MapEditorFrame;

class DbConnector;

class LoadMapDialog : public Forms::LoadMapDialog
{
private:
    MapEditorFrame* m_parentFrame;

public:
    LoadMapDialog(MapEditorFrame* parent);

    void loadMapsListFromDb(DbConnector& connector);

private:
    virtual void OnClose(wxCloseEvent& event) override;
    virtual void OnAccept(wxCommandEvent& event) override;
	virtual void OnCancel(wxCommandEvent& event) override;

	void enableMainFrameAndCloseDialog();
};
