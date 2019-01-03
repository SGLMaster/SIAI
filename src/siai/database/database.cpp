#include "mysqlconn.h"
#include "database.h"

std::unique_ptr<DBConnector> DBConnector::makeConnector(DBConnectionOptions options)
{
    return std::make_unique<MySQLConnector>(options);
}

const char* DBConnectionException::what() const noexcept
{
    return "";
}
