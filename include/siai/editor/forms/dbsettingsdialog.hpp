#pragma once

#include <editor/forms/mapeditor.h>

class MapEditorFrame;

class DbSettingsDialog: public Forms::DbSettingsDialog
{
private:
    MapEditorFrame* m_parentFrame;

public:
    DbSettingsDialog(MapEditorFrame* parent);

private:
    virtual void OnClose( wxCloseEvent& event ) override;
    virtual void OnButtonClickAccept( wxCommandEvent& event ) override;
	virtual void OnButtonClickCancel( wxCommandEvent& event ) override;

	void enableMainFrameAndCloseDialog();
};
