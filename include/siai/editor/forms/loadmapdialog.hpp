#pragma once

#include <editor/forms/mapeditor.h>

class MapEditorFrame;

class LoadMapDialog : public Forms::LoadMapDialog
{
private:
    MapEditorFrame* m_parentFrame;

public:
    LoadMapDialog(MapEditorFrame* parent);

private:
    virtual void OnAccept(wxCommandEvent& event) override;
	virtual void OnCancel(wxCommandEvent& event) override;
};
