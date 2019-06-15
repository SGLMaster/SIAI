#include "map/task.hpp"

MapTask::MapTask(int id) : m_id{id} {}

MapTask::~MapTask() = default;

IngressTask::IngressTask(int id, int rackId) : MapTask(id), m_rackId{rackId} {}

IngressTask::~IngressTask() = default;

void IngressTask::liftRack(const MapPath& newPath)
{
    m_rackLifted = true;

    setPath(newPath);
}