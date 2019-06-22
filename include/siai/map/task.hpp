#pragma once

#include "map/common.hpp"

#include <memory>

class MapTask
{
private:
    int m_id;

    MapPath m_currentPath;

protected:
    MapTask() = delete;
    MapTask(int id);
    virtual ~MapTask();

public:
    inline int getId() const noexcept{ return m_id; }

    void setPath(const MapPath& path){ m_currentPath = path; }

    virtual bool isIngressTask() const noexcept = 0;
    
    virtual MapPosition getDestination() const noexcept;
    virtual MapDirection getNextDirection(const MapPosition& currentPosition) const noexcept;

    virtual void setRackId(int rackId) noexcept = 0;
    virtual int getRackId() const noexcept = 0;
};

class IngressTask : public MapTask
{
private:
    int m_rackId;

public:
    IngressTask() = delete;
    IngressTask(int id, int rackId);
    virtual ~IngressTask();

    virtual bool isIngressTask() const noexcept override { return true; }

    virtual void setRackId(int rackId) noexcept override{ m_rackId = rackId; }
    virtual int getRackId() const noexcept override{ return m_rackId; }
};