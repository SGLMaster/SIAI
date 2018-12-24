#include "map/mapentity.hpp"

#include "map/cellimp.hpp"

IDrawable::IDrawable() = default;
IDrawable::~IDrawable() = default;

IMapEntity::IMapEntity() = default;
IMapEntity::IMapEntity(int column, int row) : m_column{column}, m_row{row} {}
IMapEntity::~IMapEntity() = default;

ICell::ICell() = default;
ICell::ICell(int column, int row) : IMapEntity(column, row) {}
ICell::~ICell() = default;

std::unique_ptr<ICell> ICell::create(std::string type, int column, int row)
{
    if(type == "Regular")
        return std::make_unique<RegularCell>(column, row);
    else if(type == "Blocked")
        return std::make_unique<BlockedCell>(column, row);

    return nullptr;
}
