#include "map/cmd/commandimp.hpp"
#include "map/entities/mapentity.hpp"
#include "map/entities/entities.hpp"

#include "log.hpp"

#include "map/exception.hpp"

ReplaceCellCommand::ReplaceCellCommand(const MapCommand::Container& arguments)
{
    if(arguments.size() != NUMBER_OF_ARGUMENTS)
    {
        throw InvalidNumberOfArguments();
    }

    m_newCellType = arguments[NEW_CELL_TYPE];
    m_column = std::stoi(arguments[COLUMN]);
    m_row = std::stoi(arguments[ROW]);
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
	MapPosition position{m_column, m_row};

    Entities::Iterator originalCellIterator = Entities::findCellIteratorWithPosition(entities, position);

    Entities::assertCellOccupied(entities, position);

    if(!undoing)
    {
        m_originalCellType = (*originalCellIterator)->getEntityName();
    }

    Entities::createCellCopyWithDifferentType(entities, originalCellIterator, cellType);
    entities.erase(originalCellIterator);

    Entities::sortEntitiesByDrawOrder(entities);
}

AddAgvCommand::AddAgvCommand(const MapCommand::Container& arguments)
{
    if(arguments.size() != NUMBER_OF_ARGUMENTS)
    {
        throw InvalidNumberOfArguments();
    }

    m_agvType = arguments[AGV_TYPE];
    m_column = std::stoi(arguments[COLUMN]);
    m_row = std::stoi(arguments[ROW]);
}

AddAgvCommand::~AddAgvCommand() = default;

void AddAgvCommand::execute(Entities::Container& entities)
{
	MapPosition position{m_column, m_row};

    Entities::assertCellOccupied(entities, position);

    try
    {
        Entities::Pointer agv = IAgv::create(m_agvType, position);
        entities.push_back(std::move(agv));
    }
    catch(EntityException& e)
    {
        Log::warning(e.what());
    }
}

void AddAgvCommand::undo(Entities::Container& entities)
{
	MapPosition position{m_column, m_row};

    Entities::Iterator agvToErase = Entities::findAgvIteratorWithPosition(entities, position);

    entities.erase(agvToErase);
}

TurnEntityCommand::TurnEntityCommand(const MapCommand::Container& arguments)
{
	if(arguments.size() != NUMBER_OF_ARGUMENTS)
	{
		throw InvalidNumberOfArguments();
	}

	m_directionToTurn = arguments[DIRECTION];
	m_column = std::stoi(arguments[COLUMN]);
	m_row = std::stoi(arguments[ROW]);
}

TurnEntityCommand::~TurnEntityCommand() = default;

void TurnEntityCommand::execute(Entities::Container& entities)
{
	auto entityToTurn = Entities::getEntityByPosition(entities, MapPosition{m_column, m_row});

	if(m_directionToTurn == "right")
	{
		entityToTurn->turnRight();
	}
	else if(m_directionToTurn == "left")
	{
		entityToTurn->turnLeft();
	}
}

void TurnEntityCommand::undo(Entities::Container& entities)
{
	auto entityToTurn = Entities::getEntityByPosition(entities, MapPosition{m_column, m_row});

	if(m_directionToTurn == "right")
	{
		entityToTurn->turnLeft();
	}
	else if(m_directionToTurn == "left")
	{
		entityToTurn->turnRight();
	}
}
