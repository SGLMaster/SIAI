#include "database/database.hpp"

#include "database/mysqlconn.hpp"

std::unique_ptr<DbConnector> DbConnector::makeConnector(const DbConnectionOptions& options)
{
    return std::make_unique<MySqlConnector>(options);
}

DbConnectionException::DbConnectionException(const char* errorMsg) : m_errorMsg{errorMsg} {}

const char* DbConnectionException::what() const noexcept
{
    return m_errorMsg;
}
