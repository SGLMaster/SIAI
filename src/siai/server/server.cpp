#include <wx/cmdline.h>
#include <wx/intl.h>

#include "server/server.hpp"
#include "server/input.hpp"

#include "database/database.hpp"

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
    std::string password{CmdInput::getString("Password")};

    DbConnectionOptions connOptions{SIAIGlobals::DB_NAME, host, port, userName, password};

    tryToConnectDb(connOptions);
    assertDbConnected();
        
    std::unique_ptr<SIAIMap> tmpMapControl(SIAIMap::createMap(true));

    m_mapControl = std::move(tmpMapControl);

    wxPrintf(_("\nA continuacion ingrese el nombre del mapa cargar.\n\n"));
    std::string mapName{CmdInput::getString("")};

    m_mapControl->setName(mapName);
	m_mapControl->loadFromDb(*m_dbConnector);
}

void ServerControl::tryToConnectDb(const DbConnectionOptions& options)
{
    try
	{
        m_dbConnector = DbConnector::makeConnector(options);
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