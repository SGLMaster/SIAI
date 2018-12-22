#include "forms/newmapdialog.hpp"
#include "forms/mapeditorframe.hpp"

#include <wx/msgdlg.h>

NewMapDialog::NewMapDialog(MapEditorFrame* parent) : Forms::NewMapDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;
}

void NewMapDialog::OnClose(wxCloseEvent& event)
{
    closeDialogAndEnableMainFrame();
}

void NewMapDialog::OnButtonClickAccept( wxCommandEvent& event )
{
    int numberOfColumns = wxAtoi( m_textCtrlNumberOfCols->GetValue() );
    int numberOfRows = wxAtoi( m_textCtrlNumberOfRows->GetValue() );

    if(numberOfColumns <= 0 || numberOfRows <= 0)
	{
		wxMessageBox( _("No ingrese números inválidos."), _("Número Inválido") );
		return;
	}

    m_parentFrame->initializeNewMap(numberOfColumns, numberOfRows);

    closeDialogAndEnableMainFrame();
}

void NewMapDialog::OnButtonClickCancel( wxCommandEvent& event )
{
    closeDialogAndEnableMainFrame();
}

void NewMapDialog::closeDialogAndEnableMainFrame()
{
    m_parentFrame->Enable();
    Destroy();
}
