#include "editor/forms/dbsettingsdialog.hpp"
#include "editor/forms/mapeditorframe.hpp"

DbSettingsDialog::DbSettingsDialog(MapEditorFrame* parent) : Forms::DbSettingsDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;
}

void DbSettingsDialog::OnClose(wxCloseEvent& event)
{
	m_parentFrame->Enable();
	Destroy();
}

void DbSettingsDialog::OnButtonClickAccept( wxCommandEvent& event )
{

}

void DbSettingsDialog::OnButtonClickCancel( wxCommandEvent& event )
{

}
