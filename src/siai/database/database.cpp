#include "../../../include/siai/database/database.hpp"

#include "../../../include/siai/database/mysqlconn.hpp"

std::unique_ptr<DbConnector> DbConnector::makeConnector(DbConnectionOptions options)
{
    return std::make_unique<MySqlConnector>(options);
}

const char* DbConnectionException::what() const noexcept
{
    return "";
}
