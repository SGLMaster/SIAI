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
