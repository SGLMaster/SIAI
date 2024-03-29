#pragma once

#include "map/cmd/command.hpp"
#include "map/entities/entities.hpp"
#include "map/entities/mapentity.hpp"
#include "painter/painter.hpp"

class ReplaceCellCommand : public MapCommand
{
private:
    enum Args
    {
        NEW_CELL_TYPE,
		MAP_NAME,
        COLUMN,
        ROW,
        NUMBER_OF_ARGUMENTS
    };

    std::string m_newCellType;
    std::string m_mapName;

    std::string m_originalCellType;

    MapPosition m_position;

public:
    ReplaceCellCommand() = delete;
    ReplaceCellCommand(const MapCommand::Container& arguments);
    virtual ~ReplaceCellCommand();

    virtual void execute(Entities::Stock& entities, DbConnector& connector) override;
    virtual void undo(Entities::Stock& entities, DbConnector& connector) override;
private:
    void doReplaceCell(Entities::Stock& entities, DbConnector& connector, bool undoing);
};

class AddAgvCommand : public MapCommand
{
private:
    enum Args
    {
        AGV_TYPE,
		MAP_NAME,
        COLUMN,
        ROW,
        NUMBER_OF_ARGUMENTS
    };

    std::string m_agvType;
    std::string m_mapName;

    MapPosition m_position;

public:
    AddAgvCommand() = delete;
    AddAgvCommand(const MapCommand::Container& arguments);
    virtual ~AddAgvCommand();

    virtual void execute(Entities::Stock& entities, DbConnector& connector) override;
    virtual void undo(Entities::Stock& entities, DbConnector& connector) override;
};

class AddRackCommand : public MapCommand
{
private:
    enum Args
    {
        RACK_TYPE,
		MAP_NAME,
        COLUMN,
        ROW,
        NUMBER_OF_ARGUMENTS
    };

    std::string m_rackType;
    std::string m_mapName;

    MapPosition m_position;

public:
    AddRackCommand() = delete;
    AddRackCommand(const MapCommand::Container& arguments);
    virtual ~AddRackCommand();

    virtual void execute(Entities::Stock& entities, DbConnector& connector) override;
    virtual void undo(Entities::Stock& entities, DbConnector& connector) override;
};

class TurnEntityCommand : public MapCommand
{
private:
    enum Args
    {
    	DIRECTION,
		MAP_NAME,
        COLUMN,
        ROW,
        NUMBER_OF_ARGUMENTS
    };

    MapPosition m_position;

    std::string m_mapName;
    std::string m_directionToTurn;

public:
    TurnEntityCommand() = delete;
    TurnEntityCommand(const MapCommand::Container& arguments);
    virtual ~TurnEntityCommand();

    virtual void execute(Entities::Stock& entities, DbConnector& connector) override;
    virtual void undo(Entities::Stock& entities, DbConnector& connector) override;
};
