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
        return "Numero de argumentos invalidos.";
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

class NotParkingCellException : public CommandException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Esta celda no es de estacionamiento.";
    }
};

class EntityException : public std::exception{};

class InvalidEntityType : public EntityException
{
public:
    virtual const char* what() const noexcept override
    {
        return "Tipo de entidad invalido.";
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
