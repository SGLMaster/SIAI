#include "map/entities/mapentity.hpp"
#include "map/entities/cellimp.hpp"
#include "map/entities/agvimp.hpp"

IDrawable::IDrawable() = default;
IDrawable::~IDrawable() = default;

IMapEntity::IMapEntity() = default;
IMapEntity::IMapEntity(int id, const MapPosition& position) : m_id{id}, m_position {position} {}
IMapEntity::~IMapEntity() = default;

ICell::ICell() = default;
ICell::ICell(int id, const MapPosition& position) : IMapEntity(id, position) {}
ICell::~ICell() = default;

std::unique_ptr<ICell> ICell::create(std::string type, int id, const MapPosition& position)
{
    if(type == "RegularCell")
        return std::make_unique<RegularCell>(id, position);
    else if(type == "BlockedCell")
        return std::make_unique<BlockedCell>(id, position);

    return nullptr;
}

IAgv::IAgv() = default;
IAgv::IAgv(int id, const MapPosition& position) : IMapEntity(id, position) {}
IAgv::~IAgv() = default;

std::unique_ptr<IAgv> IAgv::create(std::string type, int id, const MapPosition& position)
{
    if(type == "RegularAgv")
        return std::make_unique<RegularAgv>(id, position);

    return nullptr;
}
