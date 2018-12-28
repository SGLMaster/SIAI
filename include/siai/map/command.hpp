#pragma once

#include <memory>
#include <string>

class CommandDrawAll;

class MapCommand
{
public:
    static std::unique_ptr<MapCommand> create(const std::string& command, auto&&... params)
    {
        if(command == "draw-all")
        {
            return std::make_unique<CommandDrawAll>(std::forward<decltype(params)>(params)...);
        }

        return nullptr;
    }

    MapCommand();
    virtual ~MapCommand();

    virtual void execute() = 0;
    virtual void undo() = 0;
};
