#pragma once

#include <exception>

class CommandException : public std::exception{};

struct CellNotFoundException : public CommandException
{
public:
    const char* what() noexcept
    {
        return "Cell Not Found";
    }
};
