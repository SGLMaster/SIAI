#pragma once

#include <memory>
#include <string>

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

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept = 0;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept = 0;

    inline bool isSelected() const noexcept
    {
        return m_selected;
    }

    inline void select() noexcept
    {
        m_selected = true;
    }
    inline void diselect() noexcept
    {
        m_selected = false;
    }

};

enum class MapDirection
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

struct MapPosition
{
    int column;
    int row;

    MapDirection direction;
};

class IMapEntity : public IDrawable
{
protected:
    int m_id;

    MapPosition m_position;

public:
    IMapEntity();
    IMapEntity(int id, const MapPosition& position);
    virtual ~IMapEntity();

    inline int getId() const noexcept
    {
        return m_id;
    }

    inline MapPosition getPosition() const noexcept
    {
        return m_position;
    }

    inline void setPosition(int column, int row)
    {
        m_position.column = column;
        m_position.row = row;
    }

    inline void setPosition(const MapPosition& position)
    {
        m_position = position;
    }
};

class ICell : public IMapEntity
{
public:
    static std::unique_ptr<ICell> create(std::string type, int id, const MapPosition& position);

    ICell();
    ICell(int id, const MapPosition& position);
    virtual ~ICell();
};

class IAgv : public IMapEntity
{
public:
    static std::unique_ptr<IAgv> create(std::string type, int id, const MapPosition& position);

    IAgv();
    IAgv(int id, const MapPosition& position);
    virtual ~IAgv();
};
