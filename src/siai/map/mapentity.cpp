#include "map/mapentity.hpp"

DrawableEntity::DrawableEntity() = default;
DrawableEntity::~DrawableEntity() = default;

MapEntity::MapEntity() = default;
MapEntity::MapEntity(int column, int row) : m_column{column}, m_row{row} {}
MapEntity::~MapEntity() = default;
