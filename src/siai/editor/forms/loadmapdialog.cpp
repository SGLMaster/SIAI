#include "editor/forms/loadmapdialog.hpp"
#include "editor/forms/mapeditorframe.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "util/string.hpp"

#include "globals.hpp"

#include <mysql++.h>

LoadMapDialog::LoadMapDialog(MapEditorFrame* parent) : Forms::LoadMapDialog( (wxFrame *) NULL )
{
    m_parentFrame = parent;
}

void LoadMapDialog::loadMapsListFromDb(DbConnector& connector)
{
    SqlShowTablesQuery showTablesQuery;

    std::vector<DbRow> tablesList;
    connector.executeQueryAndStoreInVector(showTablesQuery, tablesList);

    std::string mapNamePrefix(SIAIGlobals::DB_CELLS_TABLE_PREFIX);

    for (const DbRow& row : tablesList) 
    {
        std::string mapName;

        row[0].to_string(mapName);

        if(Util::String::startsWith(mapName, mapNamePrefix))
        {
            mapName.replace(mapName.find(mapNamePrefix), mapNamePrefix.length(), "");
            m_choiceMapName->Append(mapName);
        }
    }
    
    if(m_choiceMapName->GetCount() > 0)
        m_choiceMapName->SetSelection(0);
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
