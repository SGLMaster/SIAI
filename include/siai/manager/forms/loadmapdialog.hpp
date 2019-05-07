#pragma once

#include <manager/forms/manager.h>
#include "database/database.hpp"

class ManagerFrame;

class LoadMapDialog : public Forms::LoadMapDialog
{
private:
    ManagerFrame* m_parentFrame;

public:
    LoadMapDialog(ManagerFrame* parent);

    void loadMapsListFromDb(DbConnector& connector);

private:
    void addMapsToChoice(const std::vector<DbRow>& tablesList);

    virtual void OnClose(wxCloseEvent& event) override;
    virtual void OnAccept(wxCommandEvent& event) override;
	virtual void OnCancel(wxCommandEvent& event) override;

	void enableMainFrameAndCloseDialog();
};
