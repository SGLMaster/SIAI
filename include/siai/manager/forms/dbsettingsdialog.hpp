#pragma once

#include <manager/forms/manager.h>

class ManagerFrame;

class DbSettingsDialog: public Forms::DbSettingsDialog
{
private:
    ManagerFrame* m_parentFrame;

public:
    DbSettingsDialog(ManagerFrame* parent);

private:
    virtual void OnClose( wxCloseEvent& event ) override;
    virtual void OnButtonClickAccept( wxCommandEvent& event ) override;
	virtual void OnButtonClickCancel( wxCommandEvent& event ) override;

	void enableMainFrameAndCloseDialog();
};
