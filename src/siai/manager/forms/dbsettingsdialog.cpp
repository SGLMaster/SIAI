#include "database/database.hpp"

#include "manager/forms/dbsettingsdialog.hpp"
#include "manager/forms/managerframe.hpp"

#include "util/database.hpp"

#include "globals.hpp"

DbSettingsDialog::DbSettingsDialog(ManagerFrame* parent) : Forms::DbSettingsDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;

	DbConnectionOptions tmpOptions = Util::Db::loadDbOptionsFromFile(SIAIGlobals::DB_CONFIG_FILENAME);

	m_textCtrlPort->SetValue(std::to_string(tmpOptions.port));
	m_textCtrlHost->SetValue(tmpOptions.host);
	m_textCtrlUser->SetValue(tmpOptions.user);
	m_textCtrlPassword->SetValue(tmpOptions.password);
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

	Util::Db::saveDbOptionsToFile(options, SIAIGlobals::DB_CONFIG_FILENAME);

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
