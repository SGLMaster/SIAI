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

    virtual void executeAndLog(Entities::Container& entities, DbConnector& connector, const std::string& command) = 0;
    virtual void undo(Entities::Container& entities) = 0;
    virtual void redo(Entities::Container& entities) = 0;
};
