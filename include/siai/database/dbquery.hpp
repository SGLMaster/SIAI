#pragma once

#include <string>
#include <vector>
#include <exception>

class DbQuery
{
public:
    DbQuery() = default;
    virtual ~DbQuery() = default;

    virtual std::string generateString() const = 0;
};

class DbQueryException : public std::exception
{
private:
	const char* m_errorMsg;

public:
	DbQueryException(const char* errorMsg) : m_errorMsg{errorMsg} {}

    virtual const char* what() const noexcept override
    {
    	return m_errorMsg;
    }
};
