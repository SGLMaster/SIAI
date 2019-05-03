#pragma once

#include <string>
#include <memory>
#include <exception>
#include <vector>

class DbQuery;

namespace mysqlpp
{
	class Row;
}

struct DbConnectionOptions
{
    std::string schema;
    std::string host;
    unsigned int port;
    std::string user;
    std::string password;
};

class DbConnector
{
public:
    DbConnector();
    virtual ~DbConnector();

    static std::unique_ptr<DbConnector> makeConnector(const DbConnectionOptions& options);

    virtual bool isConnected() const noexcept = 0;

    virtual void executeQueryWithoutResults(const DbQuery& query) = 0;
    virtual void executeQueryAndStoreInVector(const DbQuery& query, std::vector<mysqlpp::Row>& vector) = 0;
};

class DbConnectionException : public std::exception
{
private:
	const char* m_errorMsg;

public:
	DbConnectionException(const char* errorMsg);

    virtual const char* what() const noexcept override;
};
