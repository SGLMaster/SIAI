#include "map/entities/cell.hpp"
#include "map/entities/cellimp.hpp"

#include "map/exception.hpp"

Util::IdManager ICell::CellsIdManager = Util::IdManager{MIN_CELL_ID, MAX_CELL_ID};

ICell::ICell(int id, const MapPosition& position) : IMapEntity(id, position) {}
ICell::~ICell(){};

std::unique_ptr<ICell> ICell::create(std::string type, int id, const MapPosition& position)
{
    if(type == "RegularCell")
    {
    	return std::make_unique<RegularCell>(id, position);
    }

    else if(type == "BlockedCell")
    {
    	return std::make_unique<BlockedCell>(id, position);
    }

    throw InvalidEntityType();
}
