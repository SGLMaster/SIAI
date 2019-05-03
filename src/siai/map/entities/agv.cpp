#include "map/entities/agv.hpp"
#include "map/entities/agvimp.hpp"

#include "map/exception.hpp"

#include "log.hpp"

Util::IdManager IAgv::AgvsIdManager = Util::IdManager{MIN_AGV_ID, MAX_AGV_ID};

const std::vector<std::string> IAgv::dbColumnNames{ "id", "column", "row", "direction" };
const std::vector<std::string> IAgv::dbColumnTypes{ "INT NOT NULL", "INT NOT NULL", "INT NOT NULL", "INT NOT NULL" };
const std::string IAgv::primaryKeyName{"id"};

IAgv::IAgv(int id, const MapPosition& position) : IMapEntity(id, position) {}

IAgv::~IAgv()
{
    AgvsIdManager.returnId(m_id);
}

std::unique_ptr<IAgv> IAgv::create(std::string type, int id, const MapPosition& position)
{
    if(type == "RegularAgv")
    {
    	try
    	{
    		auto agv = std::make_unique<RegularAgv>(id, position);

        	return agv;
    	}
    	catch(Util::IdsNotAvailable& e)
    	{
    		Log::warning(e.what());
    	}
    }

    throw InvalidEntityType();
}
