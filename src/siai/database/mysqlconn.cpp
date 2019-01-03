#include "mysqlconn.h"
#include "dbquery.h"

MySQLConnector::MySQLConnector(DBConnectionOptions options) :
    m_connection(std::make_unique<mysqlpp::Connection>(false))
{
    bool isConnectionSuccess = m_connection->connect(  options.schema.c_str(), options.host.c_str(), options.user.c_str(),
                                                    options.password.c_str(), options.port);

    logConnectionSuccessStatus(isConnectionSuccess);
}

void MySQLConnector::logConnectionSuccessStatus(bool isSuccess) const noexcept
{
    if(isSuccess)
    {
        std::cout << "Conexión a base de Datos Correcta\n";
    }
    else
    {
        std::cout << "Problema al conectar Base de Datos: " << m_connection->error() << "\n";
    }
}

bool MySQLConnector::executeQueryWithoutResults(DBQuery& query)
{
    auto queryOutput = m_connection->query();

    std::string strQuery = query.generateString();
    queryOutput << strQuery;

    return queryOutput.exec();
}
