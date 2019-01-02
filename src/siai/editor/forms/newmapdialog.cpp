#include "editor/forms/newmapdialog.hpp"
#include "editor/forms/mapeditorframe.hpp"

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
    try
    {
        validateColsAndRowsNumbersAndInitializeNewMap();
        closeDialogAndEnableMainFrame();
    }
    catch(const InvalidNumberException& e)
    {
        wxMessageBox(_("Por favor, ingrese valores válidos para generar el mapa."), _("Número Inválido"));
    }
    catch(...)
    {
        wxMessageBox(_("Error desconocido a la hora de generar el mapa!"), _("Error Fatal"));
    }
}

void NewMapDialog::OnButtonClickCancel( wxCommandEvent& event )
{
    closeDialogAndEnableMainFrame();
}

void NewMapDialog::validateColsAndRowsNumbersAndInitializeNewMap()
{
    int numberOfColumns = wxAtoi( m_textCtrlNumberOfCols->GetValue() );
    int numberOfRows = wxAtoi( m_textCtrlNumberOfRows->GetValue() );

    if(numberOfColumns <= 0 || numberOfRows <= 0)
	{
		throw InvalidNumberException();
	}

    m_parentFrame->initializeNewMap(numberOfColumns, numberOfRows);
}

void NewMapDialog::closeDialogAndEnableMainFrame()
{
    m_parentFrame->Enable();
    Destroy();
}
