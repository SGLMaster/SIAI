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
    wxPrintf("Bienvenido al Asistente para la configuracion del Servidor de Almacen SIAI.\n");
    wxPrintf("A continuacion ingrese los datos solicitados para conectar a la base de datos.\n\n");

    std::string host{CmdInput::getString("Host")};
    unsigned int port = CmdInput::getUInt("Port");
    std::string userName{CmdInput::getString("User")};

    wxTextFile iniFile(wxT("server.ini"));

    iniFile.Open();

    iniFile.AddLine(wxString("host = ") + wxString(host));
    iniFile.AddLine(wxString( "port = ") + wxString(std::to_string(port)) );
    iniFile.AddLine(wxString("username = ") + wxString(userName));

    iniFile.Write();
    iniFile.Close();

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