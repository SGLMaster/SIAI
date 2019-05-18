#pragma once

namespace SIAIGlobals
{
    constexpr int DISTANCE_BETWEEN_CELL_AND_AGV_PX = 5;
    constexpr int AGVS_DEFAULT_WIDTH_PX = 30;
    constexpr int CELLS_DEFAULT_WIDTH_PX = (AGVS_DEFAULT_WIDTH_PX*1.5) + (DISTANCE_BETWEEN_CELL_AND_AGV_PX*2);
    constexpr int RACK_DEFAULT_LENGTH = CELLS_DEFAULT_WIDTH_PX;

    constexpr char DB_NAME[] = "siai_schema";

    constexpr char DB_CELLS_TABLE_PREFIX[] = "siaimap_cells_";
    constexpr char DB_AGVS_TABLE_PREFIX[] = "siaimap_agvs_";
}
