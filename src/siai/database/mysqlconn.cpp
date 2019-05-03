#include "database/mysqlconn.hpp"

#include "database/dbquery.hpp"

#include <mysql++.h>

MySqlConnector::MySqlConnector(const DbConnectionOptions& options) :
    m_connection(std::make_unique<mysqlpp::Connection>(true))
{
    tryToConnect(options);
}

MySqlConnector::~MySqlConnector() = default;

bool MySqlConnector::isConnected() const noexcept
{
	return m_connection->connected();
}

void MySqlConnector::executeQueryWithoutResults(const DbQuery& query)
{
	auto queryOutput = m_connection->query();

	std::string strQuery = query.generateString();
	queryOutput << strQuery;

	queryOutput.exec();
}

void MySqlConnector::executeQueryAndStoreInVector(const DbQuery& query, std::vector<mysqlpp::Row>& vector)
{
	auto queryOutput = m_connection->query();

	std::string strQuery = query.generateString();
	queryOutput << strQuery;

	queryOutput.storein(vector);
}

void MySqlConnector::tryToConnect(const DbConnectionOptions& options)
{
	try
	{
		m_connection->connect(options.schema.c_str(), options.host.c_str(), options.user.c_str(),
				options.password.c_str(), options.port);
	}
	catch(const mysqlpp::ConnectionFailed& e)
	{
		throw DbConnectionException(e.what());
	}
}
