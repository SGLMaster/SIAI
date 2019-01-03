#include "database/mysqlconn.hpp"

#include "database/dbquery.hpp"

MySqlConnector::MySqlConnector(const DbConnectionOptions& options) :
    m_connection(std::make_unique<mysqlpp::Connection>(true))
{
    tryToConnect(options);
}

void MySqlConnector::executeQueryWithoutResults(const DbQuery& query)
{
    auto queryOutput = m_connection->query();

    std::string strQuery = query.generateString();
    queryOutput << strQuery;

    queryOutput.exec();
}

void MySqlConnector::tryToConnect(const DbConnectionOptions& options)
{
	try
	{
		m_connection->connect(options.schema.c_str(), options.host.c_str(), options.user.c_str(),
				options.password.c_str(), options.port);
	}
	catch(mysqlpp::ConnectionFailed& e)
	{
		throw DbConnectionException(e.what());
	}
}

void MySqlConnector::tryToExecuteQueryWithoutResults(const DbQuery& query)
{
	try
	{
		auto queryOutput = m_connection->query();

		std::string strQuery = query.generateString();
		queryOutput << strQuery;

		queryOutput.exec();
	}
	catch(mysqlpp::BadQuery& e)
	{
		throw DbQueryException(e.what());
	}

}
