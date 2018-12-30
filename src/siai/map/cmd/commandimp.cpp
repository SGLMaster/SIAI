#include "map/cmd/commandimp.hpp"
#include "map/entities/mapentity.hpp"
#include "map/entities/entities.hpp"

#include "log.hpp"

#include "map/exception.hpp"

static constexpr MapPosition uninitializedPosition{-1, -1, MapDirection::DOWN};

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
    initializePosition(entities);

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

void ReplaceCellCommand::initializePosition(Entities::Container& entities)
{
    bool positionIsUninitialized = (m_cellPosition.column == uninitializedPosition.column
                                    && m_cellPosition.row == uninitializedPosition.row);

    if(positionIsUninitialized)
    {
        m_cellPosition = Entities::findPositionWithPoint(entities, m_pointInsideCellToReplace);
    }
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
    initializePosition(entities);

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

void AddAgvCommand::initializePosition(Entities::Container& entities)
{
    bool positionIsUninitialized = (m_position.column == uninitializedPosition.column
                                    && m_position.row == uninitializedPosition.row);

    if(positionIsUninitialized)
    {
        m_position = Entities::findPositionWithPoint(entities, m_pointToAddAgv);
    }
}
