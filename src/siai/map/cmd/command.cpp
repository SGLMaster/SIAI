#include "map/cmd/commandimp.hpp"

MapCommand::MapCommand() = default;
MapCommand::~MapCommand() = default;

std::unique_ptr<MapCommand> MapCommand::create(const std::string& command, const std::deque<std::string>& arguments)
{
    if(command == "select")
    {
        return std::make_unique<SelectCommand>(arguments);
    }
    else if(command == "diselect-all")
    {
        return std::make_unique<DiselectAllCommand>();
    }
    else if(command == "replace-cell")
    {
        return std::make_unique<ReplaceCellCommand>(arguments);
    }

    return nullptr;
}
