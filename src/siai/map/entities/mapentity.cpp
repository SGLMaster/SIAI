#include "map/entities/mapentity.hpp"

IDrawable::IDrawable() = default;
IDrawable::~IDrawable() = default;

IMapEntity::IMapEntity(int id, const MapPosition& position) : m_id{id}, m_position {position},
		m_direction{MapDirection::RIGHT} {}
IMapEntity::~IMapEntity() = default;
