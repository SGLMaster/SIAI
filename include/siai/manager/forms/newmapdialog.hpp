#pragma once

#include <manager/forms/manager.h>

#include <exception>

class ManagerFrame;

class NewMapDialog : public Forms::NewMapDialog
{
private:
    ManagerFrame* m_parentFrame;

    class InvalidNumberException : public std::exception
    {
    };

public:
    NewMapDialog(ManagerFrame* parent);

private:
    virtual void OnClose( wxCloseEvent& event ) override;
    virtual void OnButtonClickAccept( wxCommandEvent& event ) override;
	virtual void OnButtonClickCancel( wxCommandEvent& event ) override;

	void validateColsAndRowsNumbersAndInitializeNewMap();
	void enableMainFrameAndCloseDialog();
};
