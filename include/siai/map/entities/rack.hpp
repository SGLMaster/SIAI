#pragma once

#include "map/entities/mapentity.hpp"

#include "util/id.hpp"

#include <vector>
#include <memory>

class IRack : public IMapEntity
{
public:
    static const std::vector<std::string> dbColumnNames;
	static const std::vector<std::string> dbColumnTypes;
	static const std::string primaryKeyName;

    static constexpr int MIN_RACK_ID{1};
    static constexpr int MAX_RACK_ID{99999};

    static Util::IdManager RacksIdManager;

    IRack() = delete;
    IRack(const MapPosition& position);
    IRack(int id, const MapPosition& position);
    virtual ~IRack();

    static std::unique_ptr<IRack> create(std::string type, int id, const MapPosition& position);
};
