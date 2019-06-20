#include "map/task.hpp"

#include "algorithms/pathfinder.hpp"

MapTask::MapTask(int id) : m_id{id} {}

MapTask::~MapTask() = default;

MapPosition MapTask::getDestination() const noexcept
{
    return m_currentPath.back();
}

MapDirection MapTask::getNextDirection(const MapPosition& currentPosition) const noexcept
{
    MapPosition nextPosition = PathFinder::getNextStep(m_currentPath, currentPosition);

    return PathFinder::getNextDirection(currentPosition, nextPosition);
}

IngressTask::IngressTask(int id, int rackId) : MapTask(id), m_rackId{rackId} {}

IngressTask::~IngressTask() = default;

void IngressTask::liftRack(const MapPath& newPath)
{
    m_rackLifted = true;

    setPath(newPath);
}