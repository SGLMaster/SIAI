#include "map/cmd/commandimp.hpp"
#include "map/entities/mapentity.hpp"
#include "map/entities/entities.hpp"

#include "log.hpp"

#include "map/exception.hpp"

static constexpr MapPosition uninitializedPosition{-1, -1, MapDirection::DOWN};

void Commands::initializePosition(Entities::Container& entities, MapPosition& position, PanelPoint& point)
{
    bool positionIsUninitialized = (position.column == uninitializedPosition.column
                                    && position.row == uninitializedPosition.row);

    if(positionIsUninitialized)
    {
        position = Entities::findPositionWithPoint(entities, point);
    }
}

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
    Commands::initializePosition(entities, m_cellPosition, m_pointInsideCellToReplace);

    Entities::Iterator originalCellIterator = Entities::findCellIteratorWithPosition(entities, m_cellPosition);

    Entities::assertCellOccupied(entities, m_cellPosition);

    if(!undoing)
    {
        m_originalCellType = (*originalCellIterator)->getEntityName();
    }

    Entities::createCellCopyWithDifferentType(entities, originalCellIterator, cellType);
    entities.erase(originalCellIterator);

    Entities::sortEntitiesByDrawOrder(entities);
}

AddAgvCommand::AddAgvCommand(const MapCommand::Container& arguments) : m_position{uninitializedPosition}
{
    if(arguments.size() != NUMBER_OF_ARGUMENTS)
    {
        throw InvalidNumberOfArguments();
    }

    m_agvType = arguments[AGV_TYPE];

    int pointX = std::stoi(arguments[POINT_X]);
    int pointY = std::stoi(arguments[POINT_Y]);

    m_pointToAddAgv = PanelPoint{pointX, pointY};
}

AddAgvCommand::~AddAgvCommand() = default;

void AddAgvCommand::execute(Entities::Container& entities)
{
    Commands::initializePosition(entities, m_position, m_pointToAddAgv);

    Entities::assertCellOccupied(entities, m_position);

    try
    {
        Entities::Pointer agv = IAgv::create(m_agvType, m_position);
        entities.push_back(std::move(agv));
    }
    catch(EntityException& e)
    {
        Log::warning(e.what());
    }
}

void AddAgvCommand::undo(Entities::Container& entities)
{
    Entities::Iterator agvToErase = Entities::findAgvIteratorWithPosition(entities, m_position);

    entities.erase(agvToErase);
}
