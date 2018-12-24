#pragma once

#include "map/mapentity.hpp"
#include "map/painter.hpp"

class CellDefault : public ICell
{
protected:
    PanelPoint m_origin;
    int m_sideLength;

public:
    CellDefault();
    CellDefault(int column, int row);
    virtual ~CellDefault();

    virtual void setPosition(int column, int row);

    virtual void draw(Painter& painter) override;

    virtual void setSelected() override;
    virtual void diselect() noexcept override;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept override;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept override;

protected:
    void calculateZoomedSideLength(int zoom);
    void calculateOrigin();

    void setDifferentPenIfSelected(Painter& painter);
};

class RegularCell : public CellDefault
{
public:
    RegularCell();
    RegularCell(int column, int row);
    virtual ~RegularCell();

    virtual void draw(Painter& painter) override;

};

class BlockedCell : public CellDefault
{
public:
    BlockedCell();
    BlockedCell(int column, int row);
    virtual ~BlockedCell();

    virtual void draw(Painter& painter) override;
};
