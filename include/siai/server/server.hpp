#pragma once

#include <memory>

class SIAIMap;
class DbConnector;
class DbConnectionOptions;

using DbConnectorPtr = std::unique_ptr<DbConnector>;
using MapPtr = std::unique_ptr<SIAIMap>;

class ServerControl
{
private:
    MapPtr m_mapControl;
    DbConnectorPtr m_dbConnector;

public:
    ServerControl();
    virtual ~ServerControl();

    void configure();

private:
    void tryToConnectDb(const DbConnectionOptions& options);
    void assertDbConnected();
};