#pragma once

#include <memory>
#include <string>

#include "map/entities/entities.hpp"

class CommandStream
{
public:
    CommandStream();
    virtual ~CommandStream();

    static std::unique_ptr<CommandStream> create();

    virtual void executeAndLog(Entities::Container& entities, const std::string& command) = 0;
    virtual void saveLastCommandChangeToDb(DbConnector& connector, const std::string& mapName) = 0;

    virtual void undo(Entities::Container& entities) = 0;
    virtual void redo(Entities::Container& entities) = 0;
};
