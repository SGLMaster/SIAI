#pragma once

#include <editor/forms/mapeditor.h>
#include "database/database.hpp"

class MapEditorFrame;

class LoadMapDialog : public Forms::LoadMapDialog
{
private:
    MapEditorFrame* m_parentFrame;

public:
    LoadMapDialog(MapEditorFrame* parent);

    void loadMapsListFromDb(DbConnector& connector);

private:
    void addMapsToChoice(const std::vector<DbRow>& tablesList);

    virtual void OnClose(wxCloseEvent& event) override;
    virtual void OnAccept(wxCommandEvent& event) override;
	virtual void OnCancel(wxCommandEvent& event) override;

	void enableMainFrameAndCloseDialog();
};
