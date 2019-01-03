#pragma once

#include <mysql++.h>

#include "database.h"

#include <memory>

namespace mysqlpp
{
    class Connection;
}

class MySQLConnector : public DBConnector
{
private:
    std::unique_ptr<mysqlpp::Connection> m_connection;

public:
    MySQLConnector(DBConnectionOptions options);
    virtual ~MySQLConnector() = default;

    virtual bool executeQueryWithoutResults(DBQuery& query) override;

private:
    void logConnectionSuccessStatus(bool isSuccess) const noexcept;
};
