#include "map/entities/item.hpp"
#include "map/entities/itemimp.hpp"

#include "map/exception.hpp"

#include "log.hpp"

const std::vector<std::string> AItem::dbColumnNames{"id", "code", "name", "rackid", "weight"};
const std::vector<std::string> AItem::dbColumnTypes
{
    "INT NOT NULL AUTO_INCREMENT",
    "TINYTEXT NULL",
    "TINYTEXT NULL", 
    "INT NOT NULL",
    "INT NOT NULL"
};
const std::string AItem::primaryKeyName{"id"};

AItem::AItem(const AItem::Data& data) : m_data{data} {}

AItem::~AItem(){}

std::unique_ptr<AItem> AItem::create(std::string type, const AItem::Data& data)
{
    if(type == "RegularItem")
    {
    	try
    	{
    		auto item = std::make_unique<RegularItem>(data);

        	return item;
    	}
    	catch(Util::IdsNotAvailable& e)
    	{
    		Log::warning(e.what());
    	}
    }

    throw InvalidEntityType();
}