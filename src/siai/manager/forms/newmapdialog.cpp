#include "manager/forms/newmapdialog.hpp"
#include "manager/forms/managerframe.hpp"

#include <wx/msgdlg.h>

NewMapDialog::NewMapDialog(ManagerFrame* parent) : Forms::NewMapDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;
}

void NewMapDialog::OnClose(wxCloseEvent& event)
{
	enableMainFrameAndCloseDialog();
}

void NewMapDialog::OnButtonClickAccept( wxCommandEvent& event )
{
    try
    {
        validateColsAndRowsNumbersAndInitializeNewMap();
        enableMainFrameAndCloseDialog();
    }
    catch(const InvalidNumberException& e)
    {
        wxMessageBox(_("Por favor, ingrese valores válidos para generar el mapa."), _("Número Inválido"));
    }
}

void NewMapDialog::OnButtonClickCancel( wxCommandEvent& event )
{
	enableMainFrameAndCloseDialog();
}

void NewMapDialog::validateColsAndRowsNumbersAndInitializeNewMap()
{
	std::string mapName = m_textCtrlMapName->GetValue().ToStdString();
    int numberOfColumns = wxAtoi( m_textCtrlNumberOfCols->GetValue() );
    int numberOfRows = wxAtoi( m_textCtrlNumberOfRows->GetValue() );

    if(numberOfColumns <= 0 || numberOfRows <= 0)
	{
		throw InvalidNumberException();
	}

    m_parentFrame->initializeNewMap(numberOfColumns, numberOfRows, mapName);
}

void NewMapDialog::enableMainFrameAndCloseDialog()
{
    m_parentFrame->Enable();
    Destroy();
}
