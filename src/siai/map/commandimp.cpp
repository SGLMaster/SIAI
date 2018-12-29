#include "map/commandimp.hpp"
#include "map/mapentity.hpp"
#include "map/entities.hpp"

#include "util/reversion.hpp"

#include <algorithm>

SelectCommand::SelectCommand(const MapCommand::Container& arguments)
{
    int pointX = std::stoi(arguments[POINT_X]);
    int pointY = std::stoi(arguments[POINT_Y]);

    m_point = PanelPoint{pointX, pointY};
}
SelectCommand::~SelectCommand() = default;

void SelectCommand::execute(Entities::Container& entities)
{
    for(const auto& entity : Util::reverse(entities))
    {
        bool someEntityChanged = selectOrDiselectIfHasPointInside(*entity, m_point);

        if(someEntityChanged) return;
    }
}

bool SelectCommand::selectOrDiselectIfHasPointInside(IMapEntity& entity, const PanelPoint& point) noexcept
{
    constexpr bool someEntityChanged{true};
    constexpr bool noEntityChanged{false};

    if(entity.hasPointInside(point))
    {
        if(!entity.isSelected())
            entity.select();
        else
            entity.diselect();
        return someEntityChanged;
    }

    return noEntityChanged;
}

void SelectCommand::undo(Entities::Container& entities)
{
    ;
}

GenerateMapCommand::GenerateMapCommand(const MapCommand::Container& arguments)
{
    m_numberOfColumns = std::stoi(arguments[NUMBER_OF_COLUMNS]);
    m_numberOfRows = std::stoi(arguments[NUMBER_OF_ROWS]);
}
GenerateMapCommand::~GenerateMapCommand() = default;

void GenerateMapCommand::execute(Entities::Container& entities)
{
    ;
}

void GenerateMapCommand::undo(Entities::Container& entities)
{
    ;
}

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

ReplaceCellCommand::ReplaceCellCommand(const MapCommand::Container& arguments)
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
