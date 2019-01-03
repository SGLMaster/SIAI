#pragma once

#include <mysql++.h>

#include "database.hpp"

#include <memory>

class MySqlConnector : public DbConnector
{
private:
    std::unique_ptr<mysqlpp::Connection> m_connection;

public:
    MySqlConnector(const DbConnectionOptions& options);
    virtual ~MySqlConnector() = default;

    virtual void executeQueryWithoutResults(const DbQuery& query) override;

private:
    void tryToConnect(const DbConnectionOptions& options);
    void tryToExecuteQueryWithoutResults(const DbQuery& query);
};
