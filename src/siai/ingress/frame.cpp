#include "ingress/frame.hpp"

#include "database/sqlquery.hpp"

#include "util/database.hpp"

#include "globals.hpp"
#include "log.hpp"

#include <wx/textdlg.h>

#include <mysql++.h>

IngressFrame::IngressFrame(wxWindow* parent) : Forms::IngressFrame(parent) 
{
    m_originalFrameTitle = GetTitle();

    //////////REFERENCE FOR USING CHECKLIST BOX///////////
    //m_checkListTasks->Append(wxString("Uno"));
    //m_checkListTasks->Append(wxString("Otro"));
    //m_checkListTasks->GetItem(1)->SetTextColour(wxColor(0, 255, 0));
    //m_checkListTasks->Check(1);

    updateFrameTitle();
}

IngressFrame::~IngressFrame() = default;

void IngressFrame::OnSelectionConnect(wxCommandEvent& event)
{
    m_dbConnectionOptions = Util::Db::loadDbOptionsFromFile(SIAIGlobals::DB_CONFIG_FILENAME);
    m_dbConnectionOptions.schema = SIAIGlobals::DB_NAME;

    m_mapName = wxGetTextFromUser(_("Nombre del mapa:"), _("Conectar..."), _("nuevo_mapa")).ToStdString();

    tryToConnectDb();

    updateFrame();
}

void IngressFrame::OnSelectionDisconnect(wxCommandEvent& event)
{
    m_checkListTasks->Clear();
    m_dbConnector.reset();

    updateFrame();
}

void IngressFrame::OnClickAddTask(wxCommandEvent& event)
{
    if(isDbConnected())
    {
        addTask();

        loadTasksFromDb();
    }
    else
    {
        Log::warning("Base de datos desconectada!");
    }
}

void IngressFrame::OnClickDone(wxCommandEvent& event)
{
    
}

void IngressFrame::OnToolUpdateTasks(wxCommandEvent& event)
{
    if(isDbConnected())
        loadTasksFromDb();
    else
        Log::warning("Base de datos desconectada!");
}

void IngressFrame::tryToConnectDb()
{
    try
	{
		auto connector = DbConnector::makeConnector(m_dbConnectionOptions);

		m_dbConnector = std::move(connector);
	}
	catch(const DbConnectionException& e)
	{
		Log::warning(std::string("Error al conectar a base de datos: ") + e.what());

		m_dbConnector.reset(nullptr);
	}
}

bool IngressFrame::isDbConnected()
{
    if(!m_dbConnector)
	{
		return false;
	}
    else
    {
        if(!m_dbConnector->isConnected())
		    return false;
    }

    return true;
}

void IngressFrame::loadTasksFromDb()
{
    SqlQueryData dataToSelect{SIAIGlobals::DB_INGRESS_TABLE_PREFIX + m_mapName, {"id", "code", "name", "rackid", 
                                                                                    "weight", "done"}};
    SqlSelectQuery selectQuery(dataToSelect);

    std::vector<DbRow> ingressRows;
    tryQueryAndStore(selectQuery, ingressRows);
    fillCheckList(ingressRows);
}

void IngressFrame::fillCheckList(std::vector<DbRow>& tasks)
{
    m_checkListTasks->Clear();

    for(DbRow& row : tasks)
    {
        int taskId = row[0];
        std::string taskProductCode(row[1]);
        std::string taskProductName(row[2]);
        int taskRackId = row[3];
        int taskWeight = row[4];
        bool taskDone = row[5];

        std::string taskOption = std::to_string(taskId) + " - '" + taskProductName + "' #" + taskProductCode 
                                + " - Estante: " + std::to_string(taskRackId);

        if(!taskDone)
            m_checkListTasks->Append(wxString(taskOption));

        IngressTask task{taskId, taskProductCode, taskProductName, taskRackId, taskWeight};
        m_tasksList.push_back(task);
    }
}

void IngressFrame::addTask()
{
    std::string taskCode = m_textCode->GetValue().ToStdString();
    std::string taskName = m_textName->GetValue().ToStdString();
    std::string taskRackId = m_textRack->GetValue().ToStdString();
    std::string taskWeight = m_textWeight->GetValue().ToStdString();

	std::vector<std::string> valuesToInsert{taskCode, taskName, taskRackId, taskWeight};

	std::string tableName = SIAIGlobals::DB_INGRESS_TABLE_PREFIX + m_mapName;

	SqlInsertQuery insertQuery(SqlQueryData{tableName, {"code", "name", "rackid", "weight"}, valuesToInsert});

	tryQueryWithoutResults(insertQuery);
}

void IngressFrame::tryQueryAndStore(const DbQuery& query, std::vector<DbRow>& vector)
{
    try
	{
		m_dbConnector->executeQueryAndStoreInVector(query, vector);
	}
	catch(const std::exception& e)
	{
		Log::warning(std::string("Error al enviar comando a base de datos: ") + e.what());
	}
}

void IngressFrame::tryQueryWithoutResults(const DbQuery& query)
{
    try
	{
		m_dbConnector->executeQueryWithoutResults(query);
	}
	catch(const std::exception& e)
	{
		Log::warning(std::string("Error al enviar comando a base de datos: ") + e.what());
	}
}

void IngressFrame::updateFrame()
{
    if(isDbConnected())
    {
        m_menuConnection->GetMenuItems()[0]->Enable(false); // We disable the "Connect" option.
        m_menuConnection->GetMenuItems()[1]->Enable(true);  // And enable "Disconnect".
    }
    else
    {
        m_menuConnection->GetMenuItems()[0]->Enable(true); // We enable the "Connect" option.
        m_menuConnection->GetMenuItems()[1]->Enable(false);  // And disable "Disconnect".
    }

    updateFrameTitle();
}

void IngressFrame::updateFrameTitle()
{
    std::string newFrameTitle{m_originalFrameTitle};

    if(isDbConnected())
        newFrameTitle += " [Conectado]";
    else
        newFrameTitle += " [Desconectado]";

    SetTitle(newFrameTitle);
}