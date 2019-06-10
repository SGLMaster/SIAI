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

    tryToConnectDb();
    updateFrameTitle();

    loadTasksFromDb();
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
    std::string mapName = wxGetTextFromUser(_("Nombre del mapa:"), _("Conectar..."), _("nuevo_mapa")).ToStdString();

    SqlQueryData dataToSelect{SIAIGlobals::DB_INGRESS_TABLE_PREFIX + mapName, {"id", "name", "code"}};
    SqlSelectQuery selectQuery(dataToSelect);

    std::vector<DbRow> ingressRows;
    tryQueryAndStore(selectQuery, ingressRows);
    fillCheckList(ingressRows);
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

void IngressFrame::fillCheckList(std::vector<DbRow>& tasks)
{
    for(DbRow& row : tasks)
    {
        int taskId = row[0];
        std::string taskProductName(row[1]);
        std::string taskProductCode(row[2]);

        std::string option = std::to_string(taskId) + " - " + taskProductName + " #" + taskProductCode;

        m_checkListTasks->Append(wxString(option));
    }
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