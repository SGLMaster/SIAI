#pragma once

#include "map/entities/mapentity.hpp"

#include "util/id.hpp"

#include <vector>
#include <memory>

class IRack : public IMapEntity
{
public:
    static constexpr int MIN_ID{1};
    static constexpr int MAX_ID{99999};

    static Util::IdManager IdManager;

    static const std::vector<std::string> dbColumnNames;
	static const std::vector<std::string> dbColumnTypes;
	static const std::string primaryKeyName;

    IRack() = delete;
    IRack(const MapPosition& position);
    IRack(int id, const MapPosition& position);
    virtual ~IRack();

    static std::unique_ptr<IRack> create(std::string type, int id, const MapPosition& position);

    virtual void lift() noexcept = 0;
};
