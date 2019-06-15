#pragma once

#include <vector>
#include <deque>

enum class MapDirection
{
    RIGHT,
    DOWN,
    LEFT,
    UP,
    INVALID
};

struct MapPosition
{
    int column;
    int row;
};

using MapGrid = std::vector<std::vector<int>>;
using MapPath = std::deque<MapPosition>;