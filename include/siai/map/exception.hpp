#pragma once

#include <exception>

class CommandException : public std::exception{};

class InvalidCommand : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Comando inv�lido.";
    }
};

class InvalidNumberOfArguments : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "N�mero de argumentos inv�lidos.";
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
        return "Tipo de entidad inv�lido.";
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
