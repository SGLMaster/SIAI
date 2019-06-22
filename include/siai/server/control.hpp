#pragma once

#include "map/entities/entities.hpp"

#include "database/database.hpp"

#include <memory>
#include <string>

class SIAIMap;

using DbConnectorPtr = std::unique_ptr<DbConnector>;
using MapPtr = std::unique_ptr<SIAIMap>;

class ServerControl
{
private:
    static constexpr char CMD_VAL_SEPARATOR = ':';

    MapPtr m_mapControl;

    DbConnectorPtr m_dbConnector;
    DbConnectionOptions m_dbOptions;

public:
    ServerControl();
    virtual ~ServerControl();

    void configure();
    void init();

    Entities::AgvPtr processConnection(const std::string& command);
    std::string processCommand(Entities::AgvPtr& agv, const std::string& command);
    std::string executeCommand(Entities::AgvPtr& agv, const std::string& commandName, int commandValue);

private:
    std::string commandRfid(Entities::AgvPtr& agv, int rfid);
    std::string commandDir(Entities::AgvPtr& agv, int directionValue);
    std::string commandTask(Entities::AgvPtr& agv);
    std::string commandDropTask(Entities::AgvPtr& agv);
    std::string commandNextDir(Entities::AgvPtr& agv);
    std::string commandLiftRack(Entities::AgvPtr& agv);
    std::string commandDropRack(Entities::AgvPtr& agv);

    void tryToConnectDb();
    void assertDbConnected();
};