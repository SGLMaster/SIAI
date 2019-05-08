#pragma once

#include <string>

class IConnector
{
protected:
    IConnector() = default;

public:
    virtual ~IConnector() = default;

    virtual void open() = 0;
    virtual void close() = 0;

    virtual bool isConnected() = 0;
    virtual void write(const std::string& msg) = 0;
};