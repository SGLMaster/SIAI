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
        COLUMN,
        ROW,
        NUMBER_OF_ARGUMENTS
    };

    std::string m_newCellType;
    std::string m_originalCellType;

    MapPosition m_position;

public:
    ReplaceCellCommand() = delete;
    ReplaceCellCommand(const MapCommand::Container& arguments);
    virtual ~ReplaceCellCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;

    virtual void saveChangeToDb(DbConnector& connector, const std::string& mapName) override;

private:
    void doReplaceCell(Entities::Container& entities, const std::string& cellType, bool undoingCommand);
};

class AddAgvCommand : public MapCommand
{
private:
    enum Args
    {
        AGV_TYPE,
        COLUMN,
        ROW,
        NUMBER_OF_ARGUMENTS
    };

    std::string m_agvType;

    MapPosition m_position;

public:
    AddAgvCommand() = delete;
    AddAgvCommand(const MapCommand::Container& arguments);
    virtual ~AddAgvCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;

    virtual void saveChangeToDb(DbConnector& connector, const std::string& mapName) override;
};

class TurnEntityCommand : public MapCommand
{
private:
    enum Args
    {
    	DIRECTION,
        COLUMN,
        ROW,
        NUMBER_OF_ARGUMENTS
    };

    MapPosition m_position;

    std::string m_directionToTurn;

public:
    TurnEntityCommand() = delete;
    TurnEntityCommand(const MapCommand::Container& arguments);
    virtual ~TurnEntityCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;

    virtual void saveChangeToDb(DbConnector& connector, const std::string& mapName) override;
};
