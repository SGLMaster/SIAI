#include "map/entities/itemimp.hpp"

RegularItem::RegularItem(int id, int rackId, const std::string& name) : AItem(id, rackId, name) {}
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