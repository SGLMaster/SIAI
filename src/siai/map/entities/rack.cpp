#include "map/entities/rack.hpp"
#include "map/entities/rackimp.hpp"

#include "map/exception.hpp"

#include "log.hpp"

Util::IdManager IRack::IdManager = Util::IdManager{MIN_ID, MAX_ID};

const std::vector<std::string> IRack::dbColumnNames{ "id", "column", "row", "direction" };
const std::vector<std::string> IRack::dbColumnTypes{ "INT NOT NULL", "INT NOT NULL", "INT NOT NULL", "INT NOT NULL" };
const std::string IRack::primaryKeyName{"id"};

IRack::IRack(int id, const MapPosition& position) : IMapEntity(id, position) {}

IRack::~IRack()
{
    IdManager.returnId(m_id);
}

std::unique_ptr<IRack> IRack::create(std::string type, int id, const MapPosition& position)
{
    if(type == "RegularRack")
    {
    	try
    	{
    		auto rack = std::make_unique<RegularRack>(id, position);

        	return rack;
    	}
    	catch(Util::IdsNotAvailable& e)
    	{
    		Log::warning(e.what());
    	}
    }

    throw InvalidEntityType();
}
