#pragma once

#include "map/cmd/command.hpp"
#include "map/entities/entities.hpp"
#include "map/painter/painter.hpp"

class GenerateMapCommand : public MapCommand
{
private:
    enum Args
    {
        NUMBER_OF_COLUMNS,
        NUMBER_OF_ROWS
    };

    int m_numberOfColumns;
    int m_numberOfRows;

public:
    GenerateMapCommand() = delete;
    GenerateMapCommand(const MapCommand::Container& arguments);
    virtual ~GenerateMapCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;
};

class SelectCommand : public MapCommand
{
private:
    enum Args
    {
        POINT_X,
        POINT_Y
    };

    PanelPoint m_point;

public:
    SelectCommand() = delete;
    SelectCommand(const MapCommand::Container& arguments);
    virtual ~SelectCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;

private:
    bool selectOrDiselectIfHasPointInside(IMapEntity& entity, const PanelPoint& point) noexcept;
};

class DiselectAllCommand : public MapCommand
{
public:
    DiselectAllCommand();
    virtual ~DiselectAllCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;
};

class ReplaceCellCommand : public MapCommand
{
private:
    enum Args
    {
        NEW_CELL_TYPE,
        POINT_X,
        POINT_Y
    };

    std::string m_newCellType;
    std::string m_oldCellType;

    PanelPoint m_pointInsideCellToReplace;

public:
    ReplaceCellCommand() = delete;
    ReplaceCellCommand(const MapCommand::Container& arguments);
    virtual ~ReplaceCellCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;

private:
    void doReplaceCell(Entities::Container& entities, const std::string& cellType, bool undoingCommand);
};
