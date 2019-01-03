#pragma once

#include "database/dbobject.hpp"

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

    virtual int getDrawOrder() const noexcept = 0;

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
};

enum class MapEntityType
{
    CELL,
    AGV
};

class IMapEntity : public IDrawable, public DbObject
{
protected:
    int m_id;

    MapPosition m_position;
    MapDirection m_direction;

public:
    IMapEntity() = delete;
    IMapEntity(int id, const MapPosition& position);
    virtual ~IMapEntity();

    virtual std::string getEntityName() const noexcept = 0;

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

    void turnRight() noexcept
    {
    	switch(m_direction)
    	{
    	case MapDirection::RIGHT:
    		m_direction = MapDirection::DOWN;
    		break;
    	case MapDirection::DOWN:
    		m_direction = MapDirection::LEFT;
    		break;
    	case MapDirection::LEFT:
    		m_direction = MapDirection::UP;
    		break;
    	case MapDirection::UP:
    		m_direction = MapDirection::RIGHT;
    		break;
    	}
    }

    void turnLeft() noexcept
    {
    	switch(m_direction)
    	{
    	case MapDirection::RIGHT:
    		m_direction = MapDirection::UP;
    		break;
    	case MapDirection::DOWN:
    		m_direction = MapDirection::RIGHT;
    		break;
    	case MapDirection::LEFT:
    		m_direction = MapDirection::DOWN;
    		break;
    	case MapDirection::UP:
    		m_direction = MapDirection::LEFT;
    		break;
    	}
    }
};
