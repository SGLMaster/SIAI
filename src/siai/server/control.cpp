#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/intl.h>

#include "server/control.hpp"
#include "server/input.hpp"

#define CONSOLE_APP
#include "map/siaimap.hpp"

#include "map/entities/agv.hpp"

#include "util/database.hpp"
#include "util/string.hpp"

#include "globals.hpp"
#include "log.hpp"

#include <mysql++.h>

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

Entities::AgvPtr ServerControl::processConnection(const std::string& command)
{
    using namespace Util;
    auto args = String::split<std::vector<std::string>>(command, CMD_VAL_SEPARATOR);

    std::string entityType;
    std::string entityIdStr;

    int entityId = 0;

    if(args.size() > 0)
        entityType = String::trim(args[0]);
    if(args.size() > 1)
        entityIdStr = String::trim(args[1]);

    if(entityType == "agv")
    {
        if(entityIdStr != "")
            entityId = static_cast<int>(strtol(entityIdStr.c_str(), NULL, 10));

        if(entityId != 0)
            return m_mapControl->getAgvPtr(entityId);
    }

    return NULL;
}

std::string ServerControl::processCommand(Entities::AgvPtr& agv, const std::string& command)
{
    using namespace Util;
    auto args = String::split<std::vector<std::string>>(command, CMD_VAL_SEPARATOR);

    std::string commandName;
    std::string commandValueStr;

    int commandValue = 0;

    if(args.size() > 0)
        commandName = String::trim(args[0]);
    if(args.size() > 1)
        commandValueStr = String::trim(args[1]);

    if(commandValueStr != "")
        commandValue = static_cast<int>(strtol(commandValueStr.c_str(), NULL, 10));    

    return executeCommand(agv, commandName, commandValue);
}

std::string ServerControl::executeCommand(Entities::AgvPtr& agv, const std::string& commandName, int commandValue)
{
    if(commandName == "rfid")
    {
        return commandRfid(agv, commandValue);
    }
    else if(commandName == "dir")
    {
        return commandDir(agv, commandValue);
    }
    else if(commandName == "tarea")
    {
        return commandTask(agv);
    }
    else if(commandName == "borrar-tarea")
    {
        return commandDropTask(agv);
    }
    else if(commandName == "sig-dir")
    {
        return commandNextDir(agv);
    }
    else if(commandName == "levantar")
    {
        return commandLiftRack(agv);
    }
    else if(commandName == "bajar")
    {
        return commandDropRack(agv);
    }

    return "desconocido";
}

std::string ServerControl::commandRfid(Entities::AgvPtr& agv, int rfid)
{
    bool cmdSuccess = m_mapControl->moveAgvToCellWithId(*m_dbConnector, agv, rfid);

    if (cmdSuccess)
        return "rfid OK";
    else
        return "rfid ERROR";
}

std::string ServerControl::commandDir(Entities::AgvPtr& agv, int directionValue)
{
    // We verify the received number is in the range of the possible directions
    if (directionValue >= 0 && directionValue <= 3)
    {
        MapDirection direction = static_cast<MapDirection>(directionValue);
        agv->setDirection(direction);

        // Trying to save the change in direction
        try
        {
            agv->updateInDatabase(*m_dbConnector, m_mapControl->getName());
        }
        catch (const mysqlpp::BadQuery& e)
        {
            Log::error(e.what(), true);
            return "dir ERROR";
        }

        return "dir OK";
    }

    return "dir ERROR";
}

std::string ServerControl::commandTask(Entities::AgvPtr& agv)
{
    bool cmdSuccess = m_mapControl->assignNewTaskToAgv(*m_dbConnector, agv);

    if(cmdSuccess)
        return "tarea OK";
    else
        return "tarea ERROR";
}

std::string ServerControl::commandDropTask(Entities::AgvPtr& agv)
{
    bool cmdSuccess = agv->dropTask(*m_dbConnector, m_mapControl->getName());

    if(cmdSuccess)
        return "borrar-tarea OK";
    else
        return "borrar-tarea ERROR";
}

std::string ServerControl::commandNextDir(Entities::AgvPtr& agv)
{
    std::string response = "sig-dir=";

    if(agv->isAtDestination())
    {
        bool agvHasLiftedRack = m_mapControl->hasLiftedRack(agv);

        if(agvHasLiftedRack)
        {
            response += "p";
            return response;
        }
        else
        {
            response += "l";
            return response;
        }
    }

    MapDirection nextDirection = agv->getNextDirection();

    switch (nextDirection)
    {
    case MapDirection::RIGHT:
        response += "e";
        break;
    case MapDirection::DOWN:
        response += "s";
        break;
    case MapDirection::LEFT:
        response += "o";
        break;
    case MapDirection::UP:
        response += "n";
        break;

    default:
        response += "i";
        break;
    }

    return response;
}

std::string ServerControl::commandLiftRack(Entities::AgvPtr& agv)
{
    bool liftSuccess = m_mapControl->liftRackInPosition(*m_dbConnector, agv->getPosition());
    bool updateTaskSuccess = m_mapControl->updateTaskForAgv(agv);

    if(liftSuccess && updateTaskSuccess)
    {
        return "levantar OK";
    }
    
    return "levantar ERROR";
}

std::string ServerControl::commandDropRack(Entities::AgvPtr& agv)
{
    bool dropRackSuccess = m_mapControl->dropRackInPosition(*m_dbConnector, agv->getPosition());

    if(dropRackSuccess)
    {
        return "bajar OK";
    }
    
    return "bajar ERROR";
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