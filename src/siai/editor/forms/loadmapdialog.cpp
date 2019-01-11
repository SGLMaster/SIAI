#include "editor/forms/loadmapdialog.hpp"
#include "editor/forms/mapeditorframe.hpp"

LoadMapDialog::LoadMapDialog(MapEditorFrame* parent) : Forms::LoadMapDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;
}

void LoadMapDialog::OnClose(wxCloseEvent& event)
{
	enableMainFrameAndCloseDialog();
}

void LoadMapDialog::OnAccept(wxCommandEvent& event)
{

}

void LoadMapDialog::OnCancel(wxCommandEvent& event)
{
	enableMainFrameAndCloseDialog();
}

void LoadMapDialog::enableMainFrameAndCloseDialog()
{
    m_parentFrame->Enable();
    Destroy();
}
