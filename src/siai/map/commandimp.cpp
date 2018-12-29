#include "map/commandimp.hpp"
#include "map/mapentity.hpp"
#include "map/entities.hpp"

#include <algorithm>

DiselectAllCommand::DiselectAllCommand() = default;
DiselectAllCommand::~DiselectAllCommand() = default;

void DiselectAllCommand::execute(Entities::Container& entities)
{
    for(const auto& entity : entities)
    {
        entity->diselect();
    }
}

void DiselectAllCommand::undo(Entities::Container& entities)
{
    ;
}

ReplaceCellCommand::ReplaceCellCommand(const std::deque<std::string>& arguments) : m_newCellType{""},
                                                                                    m_oldCellType{""},
                                                                                    m_pointInsideCellToReplace{0, 0}
{
    enum Arguments
    {
        NEW_CELL_TYPE,
        POINT_X,
        POINT_Y
    };

    m_newCellType = arguments[NEW_CELL_TYPE];

    int pointX = std::stoi(arguments[POINT_X]);
    int pointY = std::stoi(arguments[POINT_Y]);

    m_pointInsideCellToReplace = PanelPoint{pointX, pointY};
}

ReplaceCellCommand::~ReplaceCellCommand() = default;

void ReplaceCellCommand::execute(Entities::Container& entities)
{
    doReplaceCell(entities, m_newCellType, false);
}

void ReplaceCellCommand::undo(Entities::Container& entities)
{
    doReplaceCell(entities, m_oldCellType, true);
}

void ReplaceCellCommand::doReplaceCell(Entities::Container& entities, const std::string& cellType, bool undoingCommand)
{
    Entities::Iterator cellIterator = Entities::findCellIteratorWithPoint(entities, m_pointInsideCellToReplace);

    if(cellIterator == entities.end())
    {
        return;
    }

    const MapPosition& cellFoundPosition = (*cellIterator)->getPosition();
    bool cellFoundIsOccupied = Entities::isCellOccupied(entities, cellFoundPosition);

    if(cellFoundIsOccupied)
    {
        return;
    }

    if(!undoingCommand)
    {
        m_oldCellType = (*cellIterator)->getEntityName();
    }

    Entities::createCellCopyWithDifferentType(entities, cellIterator, cellType);
    entities.erase(cellIterator);

    Entities::sortEntitiesByDrawOrder(entities);
}
