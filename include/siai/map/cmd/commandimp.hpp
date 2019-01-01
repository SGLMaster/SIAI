#pragma once

#include "map/cmd/command.hpp"
#include "map/entities/entities.hpp"
#include "map/entities/mapentity.hpp"
#include "painter/painter.hpp"

namespace Commands
{
	static constexpr MapPosition uninitializedPosition{-1, -1, MapDirection::DOWN};

    void initializePosition(Entities::Container& entities, MapPosition& position, PanelPoint& point);
}

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

    int m_column;
    int m_row;

public:
    ReplaceCellCommand() = delete;
    ReplaceCellCommand(const MapCommand::Container& arguments);
    virtual ~ReplaceCellCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;

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

    int m_column;
    int m_row;

public:
    AddAgvCommand() = delete;
    AddAgvCommand(const MapCommand::Container& arguments);
    virtual ~AddAgvCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;
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

    int m_column;
    int m_row;

    std::string m_directionToTurn;

public:
    TurnEntityCommand() = delete;
    TurnEntityCommand(const MapCommand::Container& arguments);
    virtual ~TurnEntityCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;
};
