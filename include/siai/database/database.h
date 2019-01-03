#pragma once

#include <string>
#include <memory>
#include <exception>

class DBQuery;

struct DBConnectionOptions
{
    std::string schema;
    std::string host;
    unsigned int port;
    std::string user;
    std::string password;
};

class DBConnector
{
public:
    DBConnector() = default;
    virtual ~DBConnector() = default;

    static std::unique_ptr<DBConnector> makeConnector(DBConnectionOptions options);

    virtual bool executeQueryWithoutResults(DBQuery& query) = 0;
};

class DBConnectionException : public std::exception
{
public:
    virtual const char* what() const noexcept override;
};
