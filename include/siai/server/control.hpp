#pragma once

#include "database/database.hpp"

#include <memory>

class SIAIMap;

using DbConnectorPtr = std::unique_ptr<DbConnector>;
using MapPtr = std::unique_ptr<SIAIMap>;

class ServerControl
{
private:
    MapPtr m_mapControl;

    DbConnectorPtr m_dbConnector;
    DbConnectionOptions m_dbOptions;

public:
    ServerControl();
    virtual ~ServerControl();

    void configure();
    void run();

private:
    void saveDbOptions() const;
    void loadDbOptions();

    void tryToConnectDb();
    void assertDbConnected();
};