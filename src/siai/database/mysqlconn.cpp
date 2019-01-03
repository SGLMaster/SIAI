#include "database/mysqlconn.hpp"

#include "database/dbquery.hpp"

MySqlConnector::MySqlConnector(DbConnectionOptions options) :
    m_connection(std::make_unique<mysqlpp::Connection>(false))
{
    bool isConnectionSuccess = m_connection->connect(  options.schema.c_str(), options.host.c_str(), options.user.c_str(),
                                                    options.password.c_str(), options.port);

    logConnectionSuccessStatus(isConnectionSuccess);
}

void MySqlConnector::logConnectionSuccessStatus(bool isSuccess) const
{
    if(!isSuccess)
    {
        throw DbConnectionException();
    }
}

bool MySqlConnector::executeQueryWithoutResults(DbQuery& query)
{
    auto queryOutput = m_connection->query();

    std::string strQuery = query.generateString();
    queryOutput << strQuery;

    return queryOutput.exec();
}
