#include "map/cmd/commandimp.hpp"

#include "map/exception.hpp"

MapCommand::MapCommand() = default;
MapCommand::~MapCommand() = default;

std::unique_ptr<MapCommand> MapCommand::create(const std::string& command, const std::deque<std::string>& arguments)
{
    if(command == "replace-cell")
    {
        return std::make_unique<ReplaceCellCommand>(arguments);
    }
    else if(command == "add-agv")
    {
        return std::make_unique<AddAgvCommand>(arguments);
    }
    else if(command == "add-rack")
    {
        return std::make_unique<AddRackCommand>(arguments);
    }
    else if(command == "turn-entity")
    {
    	return std::make_unique<TurnEntityCommand>(arguments);
    }
    throw InvalidCommand();
}
