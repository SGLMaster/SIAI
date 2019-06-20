#pragma once

#include "map/common.hpp"

#include <memory>

class MapTask
{
private:
    int m_id;

    MapPath m_currentPath;

public:
    MapTask() = delete;
    MapTask(int id);
    virtual ~MapTask();

    virtual void setPath(const MapPath& path)
    {
        m_currentPath = path;
    }
    virtual MapPosition getDestination() const noexcept;
    virtual MapDirection getNextDirection(const MapPosition& currentPosition) const noexcept;
};

class IngressTask : public MapTask
{
private:
    int m_rackId;
    bool m_rackLifted = false;

public:
    IngressTask() = delete;
    IngressTask(int id, int rackId);
    virtual ~IngressTask();

    virtual void setRackId(int rackId)
    {
        m_rackId = rackId;
    }
    virtual void liftRack(const MapPath& newPath);
};