#include "map/cmd/commandimp.hpp"
#include "map/entities/mapentity.hpp"
#include "map/entities/entities.hpp"

#include "map/exception.hpp"

ReplaceCellCommand::ReplaceCellCommand(const MapCommand::Container& arguments) : m_cellPosition{uninitializedPosition}
{
    if(arguments.size() != NUMBER_OF_ARGUMENTS)
    {
        throw InvalidNumberOfArguments();
    }

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
    doReplaceCell(entities, m_originalCellType, true);
}

void ReplaceCellCommand::doReplaceCell(Entities::Container& entities, const std::string& cellType, bool undoing)
{
    Entities::Iterator originalCellIterator = findCellIterator(entities);

    assertCellOccupied(entities);

    if(!undoing)
    {
        m_originalCellType = (*originalCellIterator)->getEntityName();
    }

    Entities::createCellCopyWithDifferentType(entities, originalCellIterator, cellType);
    entities.erase(originalCellIterator);

    Entities::sortEntitiesByDrawOrder(entities);
}

Entities::Iterator ReplaceCellCommand::findCellIterator(Entities::Container& entities)
{
    bool cellPositionIsUninitialized = (m_cellPosition.column == uninitializedPosition.column
                                        && m_cellPosition.row == uninitializedPosition.row);

    if(cellPositionIsUninitialized)
    {
        Entities::Iterator cellIterator = Entities::findCellIteratorWithPoint(entities, m_pointInsideCellToReplace);

        if(cellIterator == entities.end())
        {
            throw CellNotFoundException();
        }

        m_cellPosition = (*cellIterator)->getPosition();

        return cellIterator;
    }
    else
    {
        return Entities::findCellIteratorWithPosition(entities, m_cellPosition);
    }
}

void ReplaceCellCommand::assertCellOccupied(const Entities::Container& entities) const
{
    bool cellFoundIsOccupied = Entities::isCellOccupied(entities, m_cellPosition);

    if(cellFoundIsOccupied)
    {
        throw CellOccupiedException();
    }
}
