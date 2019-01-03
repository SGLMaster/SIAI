#pragma once

namespace SIAIGlobals
{
    constexpr int DISTANCE_BETWEEN_CELL_AND_AGV_PX = 5;
    constexpr int AGVS_DEFAULT_WIDTH_PX = 30;
    constexpr int CELLS_DEFAULT_WIDTH_PX = (AGVS_DEFAULT_WIDTH_PX*1.5) + (DISTANCE_BETWEEN_CELL_AND_AGV_PX*2);
    constexpr int RACK_DEFAULT_LENGTH = CELLS_DEFAULT_WIDTH_PX;
}
