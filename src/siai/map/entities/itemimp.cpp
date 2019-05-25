#include "map/entities/itemimp.hpp"

RegularItem::RegularItem(int id, int rackId) : AItem(id, rackId) {}
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