#include "map/entities/item.hpp"
#include "map/entities/itemimp.hpp"

#include "map/exception.hpp"

#include "log.hpp"

Util::IdManager AItem::IdManager = Util::IdManager{MIN_ID, MAX_ID};

const std::vector<std::string> AItem::dbColumnNames{"id", "rackid"};
const std::vector<std::string> AItem::dbColumnTypes{"INT NOT NULL", "INT NOT NULL"};
const std::string AItem::primaryKeyName{"id"};

AItem::AItem(int id, int rackId) : m_id{id}, m_rackId{rackId} {}

AItem::~AItem()
{
    IdManager.returnId(m_id);
}

std::unique_ptr<AItem> AItem::create(std::string type, int id, int rackId)
{
    if(type == "RegularItem")
    {
    	try
    	{
    		auto item = std::make_unique<RegularItem>(id, rackId);

        	return item;
    	}
    	catch(Util::IdsNotAvailable& e)
    	{
    		Log::warning(e.what());
    	}
    }

    throw InvalidEntityType();
}