#pragma once

#include <exception>

class CommandException : public std::exception{};

class InvalidCommand : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Comando inválido.";
    }
};

class InvalidNumberOfArguments : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Número de argumentos inválidos.";
    }
};

class CellNotFoundException : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Celda no encontrada.";
    }
};

class CellOccupiedException : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Celda ocupada por una entidad.";
    }
};

class EntityException : public std::exception{};

class InvalidEntityType : public EntityException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Tipo de entidad inválido.";
    }
};

class EntityNotFound : public EntityException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Ninguna entidad seleccionada.";
    }
};
