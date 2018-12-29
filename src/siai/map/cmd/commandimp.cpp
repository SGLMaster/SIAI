#include "map/cmd/commandimp.hpp"
#include "map/entities/mapentity.hpp"
#include "map/entities/entities.hpp"

#include "map/exception.hpp"

ReplaceCellCommand::ReplaceCellCommand(const MapCommand::Container& arguments) : m_cellPosition{uninitializedPosition}
{
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
    Entities::Iterator cellIterator;

    bool cellPositionIsUninitialized = (m_cellPosition.column == uninitializedPosition.column
                                        && m_cellPosition.row == uninitializedPosition.row);

    if(cellPositionIsUninitialized)
    {
        cellIterator = Entities::findCellIteratorWithPoint(entities, m_pointInsideCellToReplace);

        if(cellIterator == entities.end())
        {
            throw CellNotFoundException();
        }

        m_cellPosition = (*cellIterator)->getPosition();
    }
    else
    {
        cellIterator = Entities::findCellIteratorWithPosition(entities, m_cellPosition);
    }

    bool cellFoundIsOccupied = Entities::isCellOccupied(entities, m_cellPosition);

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
