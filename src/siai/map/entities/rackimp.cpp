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

void RackDefault::saveToDatabase(DbConnector& connector)  const
{

}

void RackDefault::loadFromDatabase(DbConnector& connector)
{

}

CircularRack::CircularRack(const MapPosition& position) : RackDefault(position){}
CircularRack::~CircularRack() = default;

void CircularRack::draw(Painter& painter)
{

}
