#pragma once

#include "util/id.hpp"

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

    MapDirection direction;
};

enum class MapEntityType
{
    CELL,
    AGV
};

class IMapEntity : public IDrawable
{
protected:
    int m_id;

    MapPosition m_position;

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
};

class ICell : public IMapEntity
{
public:
    static constexpr int MIN_CELL_ID{1};
    static constexpr int MAX_CELL_ID{999999};

    static Util::IdManager CellsIdManager;

    ICell() = delete;
    ICell(int id, const MapPosition& position);
    virtual ~ICell();

    static std::unique_ptr<ICell> create(std::string type, int id, const MapPosition& position);

    virtual int getDrawOrder() const noexcept
    {
        return 0;
    }
};

class IAgv : public IMapEntity
{
private:
    static constexpr int MIN_AGV_ID{1};
    static constexpr int MAX_AGV_ID{9999};

    static Util::IdManager AgvsIdManager;

public:
    IAgv() = delete;
    IAgv(const MapPosition& position);
    IAgv(int id, const MapPosition& position);
    virtual ~IAgv();

    static std::unique_ptr<IAgv> create(std::string type, const MapPosition& position);

    virtual int getDrawOrder() const noexcept
    {
        return 1;
    }
};
