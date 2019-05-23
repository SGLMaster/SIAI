#pragma once

#include "map/entities/rack.hpp"
#include "painter/painter.hpp"

#include <string>

class PanelImage;

class RegularRack : public IRack
{
private:
    PanelPoint m_cellOrigin;
    PanelPoint m_center;
    int m_radio;

public:
	RegularRack() = delete;
	RegularRack(int id, const MapPosition& position);
    virtual ~RegularRack();

    virtual std::string getEntityName() const noexcept override
    {
        return "RegularRack";
    }

    virtual void draw(Painter& painter) override;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept override;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept override;

    virtual void insertToDatabase(DbConnector& connector, const std::string& mapName)  const override;
    virtual void updateInDatabase(DbConnector& connector, const std::string& mapName)  const override;
    virtual void loadFromDatabase(DbConnector& connector) override;

private:
    void calculateDrawingData(int zoom);
    void calculateZoomedRadio(int zoom);
    void calculateCellOrigin(int zoom);
    void calculateCenter(int zoom);

    void drawNormalImage(Painter& painter);
    void drawSelectedImage(Painter& painter);
};
