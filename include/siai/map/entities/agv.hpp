#pragma once

#include "map/entities/mapentity.hpp"

#include "map/task.hpp"

#include "util/id.hpp"

#include <memory>
#include <vector>
#include <string>

class IAgv : public IMapEntity
{
public:
    static constexpr int MIN_ID{1};
    static constexpr int MAX_ID{9999};

    static Util::IdManager IdManager;

    static const std::vector<std::string> dbColumnNames;
	static const std::vector<std::string> dbColumnTypes;
	static const std::string primaryKeyName;

public:
    IAgv() = delete;
    IAgv(int id, const MapPosition& position);
    virtual ~IAgv();

    static std::unique_ptr<IAgv> create(std::string type, int id, const MapPosition& position);

    virtual void assignTask(std::shared_ptr<MapTask> newTask) = 0;
    virtual void dropTask() noexcept = 0;
    virtual void updateTaskPath(const MapPath& newPath) noexcept = 0;

    virtual bool isAtDestination() const noexcept = 0;
    virtual MapDirection getNextDirection() const noexcept = 0;

    virtual int getLiftedRackId() const noexcept = 0;
};
