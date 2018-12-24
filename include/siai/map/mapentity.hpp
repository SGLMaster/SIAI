#pragma once

#include <memory>
#include <string>

class PainterContainer;
class Painter;
class PanelPoint;
class PanelSize;

class IDrawable
{
protected:
    bool m_selected{false};

    IDrawable();
    virtual ~IDrawable();

public:
    virtual void draw(Painter& painter) = 0;
    virtual void setSelected() = 0;
    virtual void diselect() noexcept = 0;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept = 0;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept = 0;
};

class IMapEntity : public IDrawable
{
protected:
    int m_column;
    int m_row;
    int m_direction;

public:
    IMapEntity();
    IMapEntity(int column, int row);
    virtual ~IMapEntity();

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

class ICell : public IMapEntity
{
public:
    ICell();
    ICell(int column, int row);
    virtual ~ICell();

    static std::unique_ptr<ICell> create(std::string type, int column, int row);
};
