#pragma once

#include <exception>

class CommandException : public std::exception{};

class CellNotFoundException : public CommandException
{
public:
    const char* what() noexcept
    {
        return "Cell Not Found.";
    }
};

class CellOccupiedException : public CommandException
{
public:
    const char* what() noexcept
    {
        return "Cell Occupied by an entity.";
    }
};
