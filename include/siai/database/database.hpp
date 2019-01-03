#pragma once

#include <string>
#include <memory>
#include <exception>

class DbQuery;

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
    DbConnector() = default;
    virtual ~DbConnector() = default;

    static std::unique_ptr<DbConnector> makeConnector(const DbConnectionOptions& options);

    virtual void executeQueryWithoutResults(DbQuery& query) = 0;
};

class DbConnectionException : public std::exception
{
public:
    virtual const char* what() const noexcept override;
};
