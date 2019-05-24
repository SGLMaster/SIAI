#pragma once

#include <memory>
#include <string>

#include "map/entities/entities.hpp"

class DbConnector;

class CommandStream
{
public:
    CommandStream();
    virtual ~CommandStream();

    static std::unique_ptr<CommandStream> create();

    virtual void executeAndLog(Entities::Stock& entities, DbConnector& connector, const std::string& command) = 0;
    virtual void undo(Entities::Stock& entities, DbConnector& connector) = 0;
    virtual void redo(Entities::Stock& entities, DbConnector& connector) = 0;

    virtual void clear() = 0;
};
