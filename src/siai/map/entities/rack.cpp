#include "map/entities/rack.hpp"
#include "map/entities/rackimp.hpp"

#include "map/exception.hpp"

#include "log.hpp"

Util::IdManager IRack::RacksIdManager = Util::IdManager{MIN_RACK_ID, MAX_RACK_ID};

IRack::IRack(int id, const MapPosition& position) : IMapEntity(id, position) {}

IRack::~IRack()
{
    RacksIdManager.returnId(m_id);
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
