#include "editor/forms/loadmapdialog.hpp"
#include "editor/forms/mapeditorframe.hpp"

LoadMapDialog::LoadMapDialog(MapEditorFrame* parent) : Forms::LoadMapDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;
}

void LoadMapDialog::OnAccept(wxCommandEvent& event)
{

}

void LoadMapDialog::OnCancel(wxCommandEvent& event)
{

}
