#include "editor/forms/loadmapdialog.hpp"
#include "editor/forms/mapeditorframe.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include <mysql++.h>

LoadMapDialog::LoadMapDialog(MapEditorFrame* parent) : Forms::LoadMapDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;
}

void LoadMapDialog::loadMapsListFromDb(DbConnector& connector)
{
    SqlShowTablesQuery showQuery;

    std::vector<DbRow> results;
    connector.executeQueryAndStoreInVector(showQuery, results);

    for (const DbRow& row : results) 
    {
        std::string tmp;

        (row[0]).to_string(tmp);

        m_choiceMapName->Append(tmp);
    }
}

void LoadMapDialog::OnClose(wxCloseEvent& event)
{
	enableMainFrameAndCloseDialog();
}

void LoadMapDialog::OnAccept(wxCommandEvent& event)
{
    std::string mapName = m_textCtrlMapName->GetValue().ToStdString();

    m_parentFrame->loadMap(mapName);

    enableMainFrameAndCloseDialog();
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
