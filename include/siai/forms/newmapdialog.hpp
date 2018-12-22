#pragma once

#include <mapeditor.h>

class MapEditorFrame;

class NewMapDialog : public Forms::NewMapDialog
{
private:
    MapEditorFrame* m_parentFrame;

public:
    NewMapDialog(MapEditorFrame* parent);

private:
    virtual void OnClose( wxCloseEvent& event ) override;
    virtual void OnButtonClickAccept( wxCommandEvent& event ) override;
	virtual void OnButtonClickCancel( wxCommandEvent& event ) override;

	void closeDialogAndEnableMainFrame();
};
