#include "database/mysqlconn.hpp"

#include "database/dbquery.hpp"

MySqlConnector::MySqlConnector(const DbConnectionOptions& options) :
    m_connection(std::make_unique<mysqlpp::Connection>(true))
{
	//This will throw if the connection doesn't succeed
    m_connection->connect(options.schema.c_str(), options.host.c_str(), options.user.c_str(), options.password.c_str(),
    		options.port);

}

void MySqlConnector::executeQueryWithoutResults(DbQuery& query)
{
    auto queryOutput = m_connection->query();

    std::string strQuery = query.generateString();
    queryOutput << strQuery;

    bool querySucceeded = queryOutput.exec();

    if(!querySucceeded)
    {
    	throw DbConnectionException();
    }
}
