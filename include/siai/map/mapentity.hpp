#pragma once

class PainterContainer;
class PanelPoint;
class PanelSize;

class DrawableEntity
{
protected:
    bool m_selected{false};

    DrawableEntity();
    virtual ~DrawableEntity();

public:
    virtual void draw(PainterContainer& painter) = 0;
    virtual void setSelected(const PanelPoint& mousePosition) = 0;
    virtual void diselect() noexcept = 0;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept = 0;
    virtual bool isUnderMouse(const PanelPoint& mousePosition) const noexcept = 0;
};

class MapEntity : public DrawableEntity
{
protected:
    int m_column;
    int m_row;
    int m_direction;

public:
    MapEntity();
    MapEntity(int column, int row);
    virtual ~MapEntity();

    virtual void setPosition(int column, int row) = 0;

    inline int getColumn() const noexcept
    {
        return m_column;
    }
    inline int getRow() const noexcept
    {
        return m_row;
    }
    inline int getDirection() const noexcept
    {
        return m_direction;
    }
};
