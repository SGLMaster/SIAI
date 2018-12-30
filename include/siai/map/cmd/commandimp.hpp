#pragma once

#include "map/cmd/command.hpp"
#include "map/entities/entities.hpp"
#include "map/entities/mapentity.hpp"
#include "map/painter/painter.hpp"

class ReplaceCellCommand : public MapCommand
{
private:
    enum Args
    {
        NEW_CELL_TYPE,
        POINT_X,
        POINT_Y,
        NUMBER_OF_ARGUMENTS
    };

    static constexpr MapPosition uninitializedPosition{-1, -1, MapDirection::DOWN};

    std::string m_newCellType;
    std::string m_originalCellType;

    PanelPoint m_pointInsideCellToReplace;

    MapPosition m_cellPosition;

public:
    ReplaceCellCommand() = delete;
    ReplaceCellCommand(const MapCommand::Container& arguments);
    virtual ~ReplaceCellCommand();

    virtual void execute(Entities::Container& entities) override;
    virtual void undo(Entities::Container& entities) override;

private:
    void doReplaceCell(Entities::Container& entities, const std::string& cellType, bool undoingCommand);

    Entities::Iterator findCellIterator(Entities::Container& entities);

    void assertCellOccupied(const Entities::Container& entities) const;

};
