#pragma once

#include "map/entities.hpp"

#include <memory>
#include <string>
#include <deque>

class MapCommand
{
public:
    static std::unique_ptr<MapCommand> create(const std::string& command, const std::deque<std::string>& arguments);

    MapCommand();
    virtual ~MapCommand();

    virtual void execute(Entities::Container& entities) = 0;
    virtual void undo(Entities::Container& entities) = 0;
};
