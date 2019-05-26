#include "map/entities/itemimp.hpp"

RegularItem::RegularItem(const AItem::Data& data) : AItem(data) {}
RegularItem::~RegularItem() = default;

void RegularItem::insertToDatabase(DbConnector& connector, const std::string& mapName) const
{
	;
}

void RegularItem::updateInDatabase(DbConnector& connector, const std::string& mapName) const
{
	;
}

void RegularItem::loadFromDatabase(DbConnector& connector)
{
	;
}