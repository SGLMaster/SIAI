#include "server/server.hpp"
#include "server/input.hpp"

#include <wx/cmdline.h>
#include <wx/intl.h>

#include "database/database.hpp"

#define CONSOLE_APP
#include "map/siaimap.hpp"

#include "globals.hpp"
#include "log.hpp"

#include <string>

void Server::configure(DbConnectorPtr& dbConnector, MapPtr& mapControl)
{
    wxPrintf(_("Bienvenido al Asistente para la configuracion del Servidor de Almacen SIAI.\n"));
    wxPrintf(_("A continuacion ingrese los datos solicitados para conectar a la base de datos.\n\n"));

    std::string host{CmdInput::getString("Host")};
    unsigned int port = CmdInput::getUInt("Port");
    std::string userName{CmdInput::getString("User")};
    std::string password{CmdInput::getString("Password")};

    DbConnectionOptions connOptions{SIAIGlobals::DB_NAME, host, port, userName, password};

    tryToConnectDb(dbConnector, connOptions);
    assertDbConnected(dbConnector);
        
    std::unique_ptr<SIAIMap> tmpMapControl(SIAIMap::createMap(true));

    mapControl = std::move(tmpMapControl);

    wxPrintf(_("\nA continuacion ingrese el nombre del mapa cargar.\n\n"));
    std::string mapName{CmdInput::getString("")};

    mapControl->setName(mapName);
	mapControl->loadFromDb(*dbConnector);

    mapControl->startServer();
}

void Server::tryToConnectDb(DbConnectorPtr& dbConnector, const DbConnectionOptions& options)
{
    try
	{
        auto tmpConnector = DbConnector::makeConnector(options);

        dbConnector = std::move(tmpConnector);
    }
    catch(const DbConnectionException& e)
    {
        Log::error(std::string("Error al conectar a base de datos: ") + e.what(), true);

        dbConnector.reset(nullptr);
    }
}

void Server::assertDbConnected(DbConnectorPtr& dbConnector)
{
    //A message is only shown if the DB is connected, if it is not, the "tryToConnectDb" method will print
    //the error messages.
    if(dbConnector)
    {
        if(dbConnector->isConnected())
            Log::simple("Conectado correctamente a la base de datos.", true);
        else
            exit(0);
    }
    else
    {
        exit(0);
    }
}