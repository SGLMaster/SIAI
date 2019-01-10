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

    int column = std::stoi(arguments[COLUMN]);
    int row = std::stoi(arguments[ROW]);

    m_position = MapPosition{column, row};
}

ReplaceCellCommand::~ReplaceCellCommand() = default;

void ReplaceCellCommand::execute(Entities::Container& entities)
{
    doReplaceCell(entities, m_newCellType, false);
}

void ReplaceCellCommand::execute(Entities::Container& entities, DbConnector& connector)
{

}

void ReplaceCellCommand::undo(Entities::Container& entities)
{
    doReplaceCell(entities, m_originalCellType, true);
}

void ReplaceCellCommand::saveChangeToDb(DbConnector& connector, const std::string& mapName)
{

}

void ReplaceCellCommand::doReplaceCell(Entities::Container& entities, const std::string& cellType, bool undoing)
{
    Entities::Iterator originalCellIterator = Entities::findCellIteratorWithPosition(entities, m_position);

    Entities::assertCellOccupied(entities, m_position);

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
    m_mapName = arguments[MAP_NAME];

    int column = std::stoi(arguments[COLUMN]);
    int row = std::stoi(arguments[ROW]);

    m_position = MapPosition{column, row};
}

AddAgvCommand::~AddAgvCommand() = default;

void AddAgvCommand::execute(Entities::Container& entities)
{
	Entities::assertPositionInsideMap(entities, m_position);
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

void AddAgvCommand::saveChangeToDb(DbConnector& connector, const std::string& mapName)
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

void TurnEntityCommand::execute(Entities::Container& entities)
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
}

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

void TurnEntityCommand::saveChangeToDb(DbConnector& connector, const std::string& mapName)
{

}
