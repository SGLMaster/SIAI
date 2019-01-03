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

    virtual void saveToDatabase(DbConnector& connector)  const override;
    virtual void loadFromDatabase(DbConnector& connector) override;
};

class CircularRack : public RackDefault
{
public:
	CircularRack() = delete;
	CircularRack(const MapPosition& position);
    virtual ~CircularRack();

    virtual std::string getEntityName() const noexcept override
    {
        return "CircularRack";
    }

    virtual void draw(Painter& painter) override;
};
