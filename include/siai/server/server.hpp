#pragma once

#include <memory>

class SIAIMap;
class DbConnector;
class DbConnectionOptions;

using DbConnectorPtr = std::unique_ptr<DbConnector>;
using MapPtr = std::unique_ptr<SIAIMap>;

namespace Server
{
    void configure(DbConnectorPtr& dbConnector, MapPtr& mapControl);

    void tryToConnectDb(DbConnectorPtr& dbConnector, const DbConnectionOptions& options);
    void assertDbConnected(DbConnectorPtr& dbConnector);
}