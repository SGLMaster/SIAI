#include "map/cmd/commandimp.hpp"
#include "map/entities/agv.hpp"
#include "map/entities/rack.hpp"
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

void ReplaceCellCommand::execute(Entities::Stock& entities, DbConnector& connector)
{
	doReplaceCell(entities, connector, false);
}

void ReplaceCellCommand::undo(Entities::Stock& entities, DbConnector& connector)
{
	doReplaceCell(entities, connector, true);
}

void ReplaceCellCommand::doReplaceCell(Entities::Stock& entities, DbConnector& connector, bool undoing)
{
    auto originalCell = Entities::getCellByPosition(entities.cells, m_position);

    Entities::assertCellOccupied(entities, m_position);

    std::string cellTypeToCreate;

    if(!undoing)
    {
        m_originalCellType = originalCell->getEntityName();
        cellTypeToCreate = m_newCellType;
    }
    else
    {
    	cellTypeToCreate = m_originalCellType;
    }

    int newId = originalCell->getId();
    MapPosition newPosition = originalCell->getPosition();

    Entities::CellPtr newCell = ICell::create(cellTypeToCreate, newId, newPosition);
    newCell->updateInDatabase(connector, m_mapName);

    entities.all.push_back(newCell);
	entities.cells.push_back(newCell);

    Entities::eraseCell(entities, originalCell->getId());
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

void AddAgvCommand::execute(Entities::Stock& entities, DbConnector& connector)
{
	Entities::assertPositionInsideMap(entities, m_position);
	Entities::assertIsParkingCell(entities, m_position);
	Entities::assertCellOccupied(entities, m_position);

	try
	{
		Entities::AgvPtr agv = IAgv::create(m_agvType, IAgv::AgvsIdManager.getId(), m_position);

		agv->insertToDatabase(connector, m_mapName);

		entities.all.push_back(agv);
		entities.agvs.push_back(agv);
	}
	catch(EntityException& e)
	{
		Log::warning(e.what());
	}
}

void AddAgvCommand::undo(Entities::Stock& entities, DbConnector& connector)
{
	Entities::eraseAgvOnDb(connector, m_mapName, m_position);
	Entities::eraseAgv(entities, m_position);
}

AddRackCommand::AddRackCommand(const MapCommand::Container& arguments)
{
    if(arguments.size() != NUMBER_OF_ARGUMENTS)
    {
        throw InvalidNumberOfArguments();
    }

    m_rackType = arguments[RACK_TYPE];
    m_mapName = arguments[MAP_NAME];

    int column = std::stoi(arguments[COLUMN]);
    int row = std::stoi(arguments[ROW]);

    m_position = MapPosition{column, row};
}

AddRackCommand::~AddRackCommand() = default;

void AddRackCommand::execute(Entities::Stock& entities, DbConnector& connector)
{
	Entities::assertPositionInsideMap(entities, m_position);
	Entities::assertIsStorageCell(entities, m_position);
	Entities::assertCellOccupied(entities, m_position);

	try
	{
		Entities::RackPtr rack = IRack::create(m_rackType, IRack::RacksIdManager.getId(), m_position);

		rack->insertToDatabase(connector, m_mapName);

		entities.all.push_back(rack);
		entities.racks.push_back(rack);
	}
	catch(EntityException& e)
	{
		Log::warning(e.what());
	}
}

void AddRackCommand::undo(Entities::Stock& entities, DbConnector& connector)
{
	Entities::eraseRackOnDb(connector, m_mapName, m_position);
	Entities::eraseRack(entities, m_position);
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

void TurnEntityCommand::execute(Entities::Stock& entities, DbConnector& connector)
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

	entityToTurn->updateInDatabase(connector, m_mapName);
}

void TurnEntityCommand::undo(Entities::Stock& entities, DbConnector& connector)
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

	entityToTurn->updateInDatabase(connector, m_mapName);
}
