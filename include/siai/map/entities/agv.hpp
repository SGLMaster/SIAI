#pragma once

#include "map/entities/mapentity.hpp"

#include "util/id.hpp"

#include <memory>
#include <vector>
#include <string>

class IAgv : public IMapEntity
{
public:
	static const std::vector<std::string> dbColumnNames;
	static const std::vector<std::string> dbColumnTypes;
	static const std::string primaryKeyName;

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
