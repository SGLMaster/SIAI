#include "database/database.hpp"

#include "manager/managerapp.hpp"
#include "manager/threads.hpp"

#include "map/siaimap.hpp"

#include <wx/thread.h>

UpdateMapThread::UpdateMapThread(SIAIMap* mapControl, DbConnector* dbConnector) : ManagerThread(),
                                                                                m_mapControl{mapControl},
                                                                                m_dbConnector{dbConnector} {}

UpdateMapThread::~UpdateMapThread()
{   
    wxCriticalSectionLocker locker(wxGetApp().m_criticalSection);

    wxArrayThread& threads = wxGetApp().m_threads;
    threads.Remove(this);

    wxGetApp().m_updateMapThread = NULL;

    if(threads.IsEmpty())
    {
        // Signal the app that there are no more threads left if it is waiting for us
        if(wxGetApp().m_shuttingDown)
        {
            wxGetApp().m_shuttingDown = false;

            wxGetApp().m_semaphoreAllDone.Post();
        }
    }
    
}

wxThread::ExitCode UpdateMapThread::Entry()
{
    while(1)
    {
        // Check if the application is shutting down
        {
            wxCriticalSectionLocker locker(wxGetApp().m_criticalSection);
            if(wxGetApp().m_shuttingDown)
                return NULL;
        }

        // Check if just this thread was asked to exit
        if(TestDestroy())
            break;

        // The important stuff
        m_mapControl->updateFromDb(*m_dbConnector);
    }
    
    return NULL;
}