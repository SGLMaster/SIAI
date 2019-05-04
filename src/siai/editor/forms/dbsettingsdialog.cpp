#include "database/database.hpp"

#include "editor/forms/dbsettingsdialog.hpp"
#include "editor/forms/mapeditorframe.hpp"

#include "globals.hpp"

DbSettingsDialog::DbSettingsDialog(MapEditorFrame* parent) : Forms::DbSettingsDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;
}

void DbSettingsDialog::OnClose(wxCloseEvent& event)
{
	enableMainFrameAndCloseDialog();
}

void DbSettingsDialog::OnButtonClickAccept( wxCommandEvent& event )
{
	unsigned int portNumber = static_cast<unsigned int>( wxAtoi( m_textCtrlPort->GetValue() ) );
	const std::string& hostName = m_textCtrlHost->GetValue().ToStdString();
	const std::string& userName = m_textCtrlUser->GetValue().ToStdString();
	const std::string& password = m_textCtrlPassword->GetValue().ToStdString();

	DbConnectionOptions options{SIAIGlobals::DB_NAME, hostName, portNumber, userName, password};

	m_parentFrame->setDbConnectionOptions(options);

	enableMainFrameAndCloseDialog();
}

void DbSettingsDialog::OnButtonClickCancel( wxCommandEvent& event )
{
	enableMainFrameAndCloseDialog();
}

void DbSettingsDialog::enableMainFrameAndCloseDialog()
{
	m_parentFrame->Enable();
	Destroy();
}
