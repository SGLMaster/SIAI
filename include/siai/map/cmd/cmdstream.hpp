#pragma once

#include <string>

class CommandStream
{
public:
    CommandStream();
    virtual ~CommandStream();

    virtual void executeAndLog(const std::string& command) = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};
