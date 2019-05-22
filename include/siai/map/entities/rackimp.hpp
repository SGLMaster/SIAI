#pragma once

#include "map/entities/rack.hpp"
#include "painter/painter.hpp"

#include <string>

class PanelImage;

class RackDefault : public IRack
{
protected:
    PanelPoint m_origin;
    PanelSize m_size;

public:
    RackDefault() = delete;
    RackDefault(const MapPosition& position);
    virtual ~RackDefault();

    virtual void draw(Painter& painter) override;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept override;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept override;

    virtual void saveToDatabase(DbConnector& connector, const std::string& mapName)  const override;
    virtual void loadFromDatabase(DbConnector& connector) override;
};

class RegularRack : public RackDefault
{
public:
	RegularRack() = delete;
	RegularRack(const MapPosition& position);
    virtual ~RegularRack();

    virtual std::string getEntityName() const noexcept override
    {
        return "RegularRack";
    }

    virtual void draw(Painter& painter) override;
};
