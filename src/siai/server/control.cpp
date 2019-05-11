#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/intl.h>

#include "server/control.hpp"
#include "server/input.hpp"

#define CONSOLE_APP
#include "map/siaimap.hpp"

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

    //All the data is registered except for the password which will be entered when the server is runned
    m_dbOptions = DbConnectionOptions{SIAIGlobals::DB_NAME, host, port, userName, ""};

    saveDbOptions();

    //std::string password{CmdInput::getString("Password")};

    //m_dbOptions = DbConnectionOptions{SIAIGlobals::DB_NAME, host, port, userName, password};

    //tryToConnectDb();
    //assertDbConnected();
        
    //std::unique_ptr<SIAIMap> tmpMapControl(SIAIMap::createMap(true));

    //m_mapControl = std::move(tmpMapControl);

    //wxPrintf(_("\nA continuacion ingrese el nombre del mapa cargar.\n\n"));
    //std::string mapName{CmdInput::getString("")};

    //m_mapControl->setName(mapName);
	//m_mapControl->loadFromDb(*m_dbConnector);
}

void ServerControl::run()
{
    Log::simple("Ejecutando el Servidor SIAI...", true);
    Log::simple("Ingrese la clave para conectar a la base de datos:", true);

    m_dbOptions.password = CmdInput::getString("");

    tryToConnectDb();
    assertDbConnected();
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