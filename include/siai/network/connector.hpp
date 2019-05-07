#pragma once

#include <string>

class IConnector
{
    protected:
        IConnector() = default;

    public:
        virtual ~IConnector() = default;

        virtual void Open() = 0;
        virtual void Close() = 0;

        virtual bool IsConnected() = 0;
        virtual void Write(const std::string& msg) = 0;

};