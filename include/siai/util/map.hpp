#pragma once

#include "globals.hpp"

namespace Util
{
	int translatePanelPointXToColumn(int pointX, int zoom)
	{
		return pointX/(SIAIGlobals::CELLS_DEFAULT_WIDTH_PX*zoom);
	}

	int translatePanelPointYToRow(int pointY, int zoom)
	{
		return pointY/(SIAIGlobals::CELLS_DEFAULT_WIDTH_PX*zoom);
	}
}
