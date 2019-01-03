#pragma once

#include <string>
#include <vector>
#include <exception>

class DBQuery
{
public:
    DBQuery() = default;
    virtual ~DBQuery() = default;

    virtual std::string generateString() const = 0;
};
