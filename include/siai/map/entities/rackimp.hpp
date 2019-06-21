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

    bool m_lifted = false;

    static std::unique_ptr<PanelImage> m_regularImage;
    static std::unique_ptr<PanelImage> m_liftedImage;
    static std::unique_ptr<PanelImage> m_selectedImage;

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

    virtual void lift() noexcept override
    {
        m_lifted = true;
    }
    virtual void drop() noexcept override
    {
        m_lifted = false;
    }

    virtual bool isLifted() const noexcept override
    {
        return m_lifted;
    }

private:
    void calculateDrawingData(int zoom);
    void calculateZoomedRadio(int zoom);
    void calculateCellOrigin(int zoom);
    void calculateCenter(int zoom);

    void drawNormalImage(Painter& painter);
    void drawSelectedImage(Painter& painter);
};
