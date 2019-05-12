#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/intl.h>

#include "server/control.hpp"
#include "server/input.hpp"

#define CONSOLE_APP
#include "map/siaimap.hpp"

#include "util/string.hpp"

#include "globals.hpp"
#include "log.hpp"

#include <string>

ServerControl::ServerControl() = default;
ServerControl::~ServerControl() = default;

void ServerControl::configure()
{
    Log::simple("Bienvenido al Asistente para la configuracion del Servidor de Almacen SIAI.", true);
    Log::simple("A continuacion ingrese los datos solicitados para conectar a la base de datos.", true);

    std::string host{CmdInput::getString("Host")};
    unsigned int port = CmdInput::getUInt("Port");
    std::string userName{CmdInput::getString("User")};

    //The data is registered except for the password which will be entered when the server is runned
    m_dbOptions = DbConnectionOptions{"", host, port, userName, ""};

    saveDbOptions();
}

void ServerControl::run()
{
    m_dbOptions.schema = SIAIGlobals::DB_NAME;
    loadDbOptions();

    Log::simple("\nEjecutando el Servidor SIAI...", true);
    Log::simple(std::string("Ingrese la clave para conectar a la base de datos con el usuario \"") 
                            + m_dbOptions.user + "\":", true);

    m_dbOptions.password = CmdInput::getString("");

    tryToConnectDb();
    assertDbConnected();

    m_mapControl = MapPtr(SIAIMap::createMap(true));

    Log::simple("\nIngrese el nombre del mapa que desea cargar:", true);

    std::string mapName{CmdInput::getString("")};

    m_mapControl->setName(mapName);
	m_mapControl->loadFromDb(*m_dbConnector);
}

void ServerControl::saveDbOptions() const
{
    wxTextFile iniFile(wxT("database.ini"));

    iniFile.Open();

    iniFile.Clear();

    iniFile.AddLine(wxString("host = ") + wxString(m_dbOptions.host));
    iniFile.AddLine(wxString("port = ") + wxString(std::to_string(m_dbOptions.port)) );
    iniFile.AddLine(wxString("username = ") + wxString(m_dbOptions.user));

    iniFile.Write();
    iniFile.Close();
}

void ServerControl::loadDbOptions()
{
    wxTextFile iniFile(wxT("database.ini"));

    iniFile.Open();

    wxString curLine;
    for(unsigned int i = 0; i < iniFile.GetLineCount(); ++i)
    {
        curLine = iniFile.GetLine(i);

        if(curLine.StartsWith("host"))
            m_dbOptions.host = Util::String::getOptValueAsStr(curLine.ToStdString());

        else if(curLine.StartsWith("port"))
        {
            m_dbOptions.port = static_cast<unsigned int>(Util::String::getOptValueAsInt(curLine.ToStdString()));
        }

        else if(curLine.StartsWith("username"))
            m_dbOptions.user = Util::String::getOptValueAsStr(curLine.ToStdString());
    }

    iniFile.Close();
}

void ServerControl::tryToConnectDb()
{
    try
	{
        m_dbConnector = DbConnector::makeConnector(m_dbOptions);
    }
    catch(const DbConnectionException& e)
    {
        Log::error(std::string("Error al conectar a base de datos: ") + e.what(), true);

        m_dbConnector.reset(nullptr);
    }
}

void ServerControl::assertDbConnected()
{
    //A message is only shown if the DB is connected, if it is not, the "tryToConnectDb" method will print
    //the error messages.
    if(m_dbConnector)
    {
        if(m_dbConnector->isConnected())
            Log::simple("Conectado correctamente a la base de datos.", true);
        else
            exit(0);
    }
    else
    {
        exit(0);
    }
}