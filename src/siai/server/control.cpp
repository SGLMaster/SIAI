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

static constexpr char INI_FILENAME[] = "server.ini";

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

    Util::Db::saveDbOptionsToFile(m_dbOptions, INI_FILENAME);
}

void ServerControl::init()
{
    m_dbOptions = Util::Db::loadDbOptionsFromFile(INI_FILENAME);
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
    std::string entityId;
    std::string commandName;

    if(args.size() > 0)
        entityType = String::trim(args[0]);
    if(args.size() > 1)
        entityId = String::trim(args[1]);
    if(args.size() > 2)
        commandName = String::trim(args[2]);

    if(entityType == "AGV")
    {
        if(entityId != "")
            return std::string("AGV ID ") + entityId + " OK";
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