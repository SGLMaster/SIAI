#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/intl.h>

#include "server/control.hpp"
#include "server/input.hpp"

#define CONSOLE_APP
#include "map/siaimap.hpp"

#include "util/database.hpp"
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
    std::string password{CmdInput::getString("Password")};

    //The data is registered except for the password which will be entered when the server is runned
    m_dbOptions = DbConnectionOptions{"", host, port, userName, password};

    Util::Db::saveDbOptionsToFile(m_dbOptions, SIAIGlobals::DB_CONFIG_FILENAME);
}

void ServerControl::init()
{
    m_dbOptions = Util::Db::loadDbOptionsFromFile(SIAIGlobals::DB_CONFIG_FILENAME);
    m_dbOptions.schema = SIAIGlobals::DB_NAME;

    Log::simple("\nEjecutando el Servidor SIAI...", true);

    //If the password wasn't set on the settings file or was left blank it can be entered here
    if(m_dbOptions.password == "")
    {
        Log::simple(std::string("Ingrese la clave para conectar a la base de datos con el usuario \"") 
                            + m_dbOptions.user + "\":", true);
        m_dbOptions.password = CmdInput::getString("");
    }
    
    tryToConnectDb();
    assertDbConnected();

    m_mapControl = MapPtr(SIAIMap::createMap(true));

    Log::simple("\nIngrese el nombre del mapa que desea cargar:", true);

    std::string mapName{CmdInput::getString("")};

    m_mapControl->setName(mapName);
	m_mapControl->loadFromDb(*m_dbConnector);
}

std::string ServerControl::processCommand(const std::string& command)
{
    using namespace Util;
    auto args = String::split<std::vector<std::string>>(command, CMD_VAL_SEPARATOR);

    std::string entityType;
    std::string entityIdStr;
    std::string commandName;
    std::string commandValueStr;

    int entityId = 0;
    int commandValue = 0;

    if(args.size() > 0)
        entityType = String::trim(args[0]);
    if(args.size() > 1)
        entityIdStr = String::trim(args[1]);
    if(args.size() > 2)
        commandName = String::trim(args[2]);
    if(args.size() > 3)
        commandValueStr = String::trim(args[3]);

    if(entityIdStr != "")
        entityId = static_cast<int>(strtol(entityIdStr.c_str(), NULL, 10));

    if(commandValueStr != "")
        commandValue = static_cast<int>(strtol(commandValueStr.c_str(), NULL, 10));    

    if(entityType == "AGV")
    {
        if(commandName == "RFID")
        {
            bool cmdSuccess = m_mapControl->moveAgvToCellWithId(*m_dbConnector, entityId, commandValue);

            if(cmdSuccess)
                return "OK";
            else
                return "ERROR";
        }
    }

    return "UNK";
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