#pragma once

#include <deque>

namespace Util
{
    class IdManager
    {
    private:
        int m_minId;
        int m_maxId;

        std::deque<int> m_idsAvailable;

    public:
        IdManager(int minId, int maxId);
        ~IdManager();

        int getId();
        void returnId(int idToReturnToGenerator);

        void reset();

    private:
        void generateIds();
    };
}
