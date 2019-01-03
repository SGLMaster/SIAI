#pragma once

#include "database.hpp"

#include <mysql++.h>

#include <memory>

namespace mysqlpp
{
    class Connection;
}

class MySqlConnector : public DbConnector
{
private:
    std::unique_ptr<mysqlpp::Connection> m_connection;

public:
    MySqlConnector(DbConnectionOptions options);
    virtual ~MySqlConnector() = default;

    virtual bool executeQueryWithoutResults(DbQuery& query) override;

private:
    void logConnectionSuccessStatus(bool isSuccess) const;
};
