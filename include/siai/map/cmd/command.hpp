#pragma once

#include "map/entities/entities.hpp"

#include <memory>
#include <string>
#include <deque>

class DbConnector;

class MapCommand
{
public:
    using Container = std::deque<std::string>;

    static std::unique_ptr<MapCommand> create(const std::string& command, const MapCommand::Container& arguments);

    MapCommand();
    virtual ~MapCommand();

    virtual void execute(Entities::Stock& entities, DbConnector& connector) = 0;
    virtual void undo(Entities::Stock& entities, DbConnector& connector) = 0;
};
