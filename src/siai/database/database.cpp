#include "database/database.hpp"

#include "database/mysqlconn.hpp"

std::unique_ptr<DbConnector> DbConnector::makeConnector(const DbConnectionOptions& options)
{
    return std::make_unique<MySqlConnector>(options);
}

const char* DbConnectionException::what() const noexcept
{
    return "No se pudo conectar con la base de datos. Verifique sus datos y la conexión.";
}
