#pragma once

#include "map/mapentity.hpp"
#include "map/painter.hpp"

#include "globals.hpp"

#include <memory>

class Cell : public MapEntity
{
protected:
    PanelPoint m_origin;
    int m_sideLength{SIAIGlobals::CELLS_DEFAULT_WIDTH_PX};

public:
    static std::unique_ptr<Cell> createRegularCell(int column, int row);
    static std::unique_ptr<Cell> createBlockedCell(int column, int row);

    Cell();
    Cell(int column, int row);
    virtual ~Cell();

    virtual void setPosition(int column, int row);

    virtual void draw(PainterContainer& painter) = 0;

    virtual void setSelected(const PanelPoint& mousePosition) override;
    virtual void diselect() noexcept override;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept override;
    virtual bool isUnderMouse(const PanelPoint& mousePosition) const noexcept override;

protected:
    void calculateZoomedSideLength(int zoom);
    void calculateOrigin();
};
