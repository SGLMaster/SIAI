#pragma once

#include "map/entities/mapentity.hpp"
#include "map/painter/painter.hpp"

class PanelImage;

class AgvDefault : public IAgv
{
protected:
    PanelPoint m_origin;
    //int m_sideLength;
    PanelSize m_size;

public:
    AgvDefault();
    AgvDefault(int id, const MapPosition& position);
    virtual ~AgvDefault();

    virtual void draw(Painter& painter) override;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept override;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept override;

protected:
    void calculateDrawingData(int zoom);
    void calculateZoomedSize(int zoom);
    void calculateOrigin(int zoom);
};

class RegularAgv : public AgvDefault
{
public:
    RegularAgv();
    RegularAgv(int id, const MapPosition& position);
    virtual ~RegularAgv();

    virtual std::string getEntityName() const noexcept override
    {
        return "RegularAgv";
    }

    virtual void draw(Painter& painter) override;

};
