#include "map/siaimap.hpp"
#include "map/siaimapimp.hpp"

SIAIMap::SIAIMap() = default;
SIAIMap::~SIAIMap() = default;

SIAIMap* SIAIMap::createMap(bool forConsole)
{
    return new SIAIMapImp(forConsole);
}
