#include "map/entities/cell.hpp"
#include "map/entities/cellimp.hpp"

#include "map/exception.hpp"

Util::IdManager ICell::IdManager = Util::IdManager{MIN_ID, MAX_ID};

const std::vector<std::string> ICell::dbColumnNames{ "id", "column", "row", "direction", "type" };
const std::vector<std::string> ICell::dbColumnTypes
{ 
    "INT NOT NULL", 
    "INT NOT NULL", 
    "INT NOT NULL", 
    "INT NOT NULL",
	"TINYTEXT NULL"
};
const std::string ICell::primaryKeyName{"id"};

ICell::ICell(int id, const MapPosition& position) : IMapEntity(id, position) {}
ICell::~ICell(){};

std::unique_ptr<ICell> ICell::create(std::string type, int id, const MapPosition& position)
{
    if(type == "Regular")
    {
    	return std::make_unique<RegularCell>(id, position);
    }

    else if(type == "Blocked")
    {
    	return std::make_unique<BlockedCell>(id, position);
    }

    else if(type == "Storage")
    {
    	return std::make_unique<StorageCell>(id, position);
    }

    else if(type == "Parking")
    {
    	return std::make_unique<ParkingCell>(id, position);
    }

    throw InvalidEntityType();
}
