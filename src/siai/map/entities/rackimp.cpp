#include "map/entities/rackimp.hpp"

#include "globals.hpp"

RackDefault::RackDefault(const MapPosition& position) : IRack(position) {}
RackDefault::~RackDefault() = default;

void RackDefault::draw(Painter& painter)
{

}

bool RackDefault::isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept
{
	return false;
}

bool RackDefault::hasPointInside(const PanelPoint& point) const noexcept
{
	return false;
}

void RackDefault::saveToDatabase(DbConnector& connector, const std::string& mapName) const
{

}

void RackDefault::loadFromDatabase(DbConnector& connector)
{

}

RegularRack::RegularRack(const MapPosition& position) : RackDefault(position){}
RegularRack::~RegularRack() = default;

void RegularRack::draw(Painter& painter)
{

}
