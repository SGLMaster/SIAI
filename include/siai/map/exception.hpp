#pragma once

#include <exception>

class CommandException : public std::exception{};

class InvalidNumberOfArguments : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Invalid number of arguments.";
    }
};

class CellNotFoundException : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Cell not found.";
    }
};

class CellOccupiedException : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Cell occupied by an entity.";
    }
};

class EntityException : public std::exception{};

class InvalidEntityType : public EntityException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Invalid entity type name.";
    }
};
