#include "ingress/frame.hpp"

#include "util/database.hpp"

#include "globals.hpp"
#include "log.hpp"

IngressFrame::IngressFrame(wxWindow* parent) : Forms::IngressFrame(parent) 
{
    m_originalFrameTitle = GetTitle();

    updateFrameTitle();
}

IngressFrame::~IngressFrame() = default;

void IngressFrame::OnSelectionConnect(wxCommandEvent& event)
{
    m_dbConnectionOptions = Util::Db::loadDbOptionsFromFile(SIAIGlobals::DB_CONFIG_FILENAME);
    m_dbConnectionOptions.schema = SIAIGlobals::DB_NAME;

    tryToConnectDb();
    updateFrameTitle();
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

void IngressFrame::updateFrameTitle()
{
    std::string newFrameTitle{m_originalFrameTitle};

    if(isDbConnected())
        newFrameTitle += " [Conectado]";
    else
        newFrameTitle += " [Desconectado]";

    SetTitle(newFrameTitle);
}