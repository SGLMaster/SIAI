#pragma once

#include <editor/forms/mapeditor.h>

#include <exception>

class MapEditorFrame;

class NewMapDialog : public Forms::NewMapDialog
{
private:
    MapEditorFrame* m_parentFrame;

    class InvalidNumberException : public std::exception
    {
    };

public:
    NewMapDialog(MapEditorFrame* parent);

private:
    virtual void OnClose( wxCloseEvent& event ) override;
    virtual void OnButtonClickAccept( wxCommandEvent& event ) override;
	virtual void OnButtonClickCancel( wxCommandEvent& event ) override;

	void validateColsAndRowsNumbersAndInitializeNewMap();
	void enableMainFrameAndCloseDialog();
};
