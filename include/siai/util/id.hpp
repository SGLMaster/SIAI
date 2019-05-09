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
        void retrieveId(int idToRetrieveFromGenerator);
        void returnId(int idToReturnToGenerator);

        void reset();

    private:
        void generateIds();
    };

    class IdsNotAvailable : public std::exception
	{
	public:
    	virtual const char* what() const noexcept override
    	{
    		return "No se pueden generar más ID's para esta clase.";
    	}
	};
}
