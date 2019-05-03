#pragma once

#include "database.hpp"

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
    MySqlConnector(const DbConnectionOptions& options);
    virtual ~MySqlConnector();

    virtual bool isConnected() const noexcept override;

    virtual void executeQueryWithoutResults(const DbQuery& query) override;
    virtual void executeQueryAndStoreInVector(const DbQuery& query, std::vector<mysqlpp::Row>& vector) override;

private:
    void tryToConnect(const DbConnectionOptions& options);
};
