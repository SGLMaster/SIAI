#include "util/id.hpp"

using namespace Util;

IdManager::IdManager(int minId, int maxId) : m_minId{minId}, m_maxId{maxId}, m_idsAvailable{}
{
    generateIds();
};

IdManager::~IdManager() = default;

int IdManager::getId()
{
    int idToDeliver{-1};

    if(m_idsAvailable.size() > 0)
    {
        idToDeliver = m_idsAvailable.front();
        m_idsAvailable.pop_front();
    }

    return idToDeliver;
}

void IdManager::returnId(int idToReturnToGenerator)
{
    m_idsAvailable.push_back(idToReturnToGenerator);
}

void IdManager::generateIds()
{
    for(int i = m_minId; i <= m_maxId; ++i)
    {
        m_idsAvailable.push_back(i);
    }
}

void IdManager::reset()
{
    m_idsAvailable.resize(0);

    generateIds();
}
