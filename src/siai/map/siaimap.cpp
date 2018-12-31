#include "map/siaimap.hpp"
#include "map/siaimapimp.hpp"

SIAIMap::SIAIMap() = default;
SIAIMap::~SIAIMap() = default;

SIAIMap* SIAIMap::createMap()
{
    return new SIAIMapImp();
}
