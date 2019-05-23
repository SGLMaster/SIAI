#pragma once

#include "map/entities/agv.hpp"
#include "painter/painter.hpp"

#include <memory>
#include <string>

class PanelImage;

class AgvDefault : public IAgv
{
protected:
    PanelPoint m_origin;
    PanelSize m_size;

public:
    AgvDefault() = delete;
    AgvDefault(int id, const MapPosition& position);
    virtual ~AgvDefault();

    virtual void draw(Painter& painter) override;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept override;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept override;

    virtual void insertToDatabase(DbConnector& connector, const std::string& mapName) const override;
    virtual void updateInDatabase(DbConnector& connector, const std::string& mapName) const override;
    virtual void loadFromDatabase(DbConnector& connector) override;

protected:
    void calculateDrawingData(int zoom);
    void calculateZoomedSize(int zoom);
    void calculateOrigin(int zoom);
};

class RegularAgv : public AgvDefault
{
public:
    RegularAgv() = delete;
    RegularAgv(int id, const MapPosition& position);
    virtual ~RegularAgv();

    virtual std::string getEntityName() const noexcept override
    {
        return "RegularAgv";
    }

    virtual void draw(Painter& painter) override;

private:
    void drawNormalImage(Painter& painter);
    void drawSelectedImage(Painter& painter);
};
