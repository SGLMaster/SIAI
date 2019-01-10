#include "map/cmd/commandimp.hpp"
#include "map/entities/agv.hpp"
#include "map/entities/cell.hpp"
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
    m_mapName = arguments[MAP_NAME];

    int column = std::stoi(arguments[COLUMN]);
    int row = std::stoi(arguments[ROW]);

    m_position = MapPosition{column, row};
}

ReplaceCellCommand::~ReplaceCellCommand() = default;


void ReplaceCellCommand::execute(Entities::Container& entities, DbConnector& connector)
{
	doReplaceCell(entities, connector, false);
}

void ReplaceCellCommand::undo(Entities::Container& entities)
{
    //doReplaceCell(entities, true);
}

void ReplaceCellCommand::undo(Entities::Container& entities, DbConnector& connector)
{

}

void ReplaceCellCommand::doReplaceCell(Entities::Container& entities, DbConnector& connector, bool undoing)
{
    Entities::Iterator originalCellIterator = Entities::findCellIteratorWithPosition(entities, m_position);

    Entities::assertCellOccupied(entities, m_position);

    if(!undoing)
    {
        m_originalCellType = (*originalCellIterator)->getEntityName();
    }

    int newId = (*originalCellIterator)->getId();
    MapPosition newPosition = (*originalCellIterator)->getPosition();

    auto newCell = ICell::create(m_newCellType, newId, newPosition);
    newCell->saveToDatabase(connector, m_mapName);

    entities.push_back(std::move(newCell));
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
    m_mapName = arguments[MAP_NAME];

    int column = std::stoi(arguments[COLUMN]);
    int row = std::stoi(arguments[ROW]);

    m_position = MapPosition{column, row};
}

AddAgvCommand::~AddAgvCommand() = default;

void AddAgvCommand::execute(Entities::Container& entities, DbConnector& connector)
{
	Entities::assertPositionInsideMap(entities, m_position);
	Entities::assertCellOccupied(entities, m_position);

	try
	{
		Entities::Pointer agv = IAgv::create(m_agvType, m_position);

		agv->saveToDatabase(connector, m_mapName);

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

void AddAgvCommand::undo(Entities::Container& entities, DbConnector& connector)
{

}

TurnEntityCommand::TurnEntityCommand(const MapCommand::Container& arguments)
{
	if(arguments.size() != NUMBER_OF_ARGUMENTS)
	{
		throw InvalidNumberOfArguments();
	}

	m_directionToTurn = arguments[DIRECTION];
	m_mapName = arguments[MAP_NAME];

	int column = std::stoi(arguments[COLUMN]);
	int row = std::stoi(arguments[ROW]);

	m_position = MapPosition{column, row};
}

TurnEntityCommand::~TurnEntityCommand() = default;

void TurnEntityCommand::execute(Entities::Container& entities, DbConnector& connector)
{
	auto entityToTurn = Entities::getEntityByPosition(entities, m_position);

	if(m_directionToTurn == "right")
	{
		entityToTurn->turnRight();
	}
	else if(m_directionToTurn == "left")
	{
		entityToTurn->turnLeft();
	}

	entityToTurn->saveToDatabase(connector, m_mapName);
}

void TurnEntityCommand::undo(Entities::Container& entities)
{
	auto entityToTurn = Entities::getEntityByPosition(entities, m_position);

	if(m_directionToTurn == "right")
	{
		entityToTurn->turnLeft();
	}
	else if(m_directionToTurn == "left")
	{
		entityToTurn->turnRight();
	}
}

void TurnEntityCommand::undo(Entities::Container& entities, DbConnector& connector)
{
	auto entityToTurn = Entities::getEntityByPosition(entities, m_position);

	if(m_directionToTurn == "right")
	{
		entityToTurn->turnLeft();
	}
	else if(m_directionToTurn == "left")
	{
		entityToTurn->turnRight();
	}

	entityToTurn->saveToDatabase(connector, m_mapName);
}
