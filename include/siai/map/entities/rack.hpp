#pragma once

#include "map/entities/mapentity.hpp"

#include "util/id.hpp"

#include <memory>

class IRack : public IMapEntity
{
private:
    static constexpr int MIN_RACK_ID{1};
    static constexpr int MAX_RACK_ID{99999};

    static Util::IdManager RacksIdManager;

public:
    IRack() = delete;
    IRack(const MapPosition& position);
    IRack(int id, const MapPosition& position);
    virtual ~IRack();

    static std::unique_ptr<IRack> create(std::string type, const MapPosition& position);

    virtual int getDrawOrder() const noexcept
    {
        return 2;
    }
};
