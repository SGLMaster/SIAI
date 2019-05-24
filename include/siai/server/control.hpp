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
    std::string processCommand(const std::string& command);

private:
    void tryToConnectDb();
    void assertDbConnected();
};