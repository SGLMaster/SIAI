#pragma once

#include "map/command.hpp"
#include "map/painter.hpp"
#include "map/entities.hpp"

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
    std::string m_newCellType;
    std::string m_oldCellType;

    PanelPoint m_pointInsideCellToReplace;

public:
    ReplaceCellCommand() = delete;
    ReplaceCellCommand(const std::deque<std::string>& arguments);
    virtual ~ReplaceCellCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;

private:
    void doReplaceCell(Entities::Container& entities, const std::string& cellType, bool undoingCommand);
};
