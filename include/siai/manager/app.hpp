#pragma once

#include <wx/thread.h>
#include <wx/app.h>

class MyThread;
WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);

class ManagerThread;
class ManagerFrame;

class ManagerApp : public wxApp
{
public:
    wxCriticalSection m_criticalSection;

    wxArrayThread m_threads;

    ManagerThread* m_updateMapThread;

    wxSemaphore m_semaphoreAllDone;

    bool m_shuttingDown;

    ManagerApp();
    virtual ~ManagerApp();

private:
    ManagerFrame* m_frame;

    virtual bool OnInit() override;

    void createAndShowSplashScreen();
    void initializeFrame();
    void loadAndSetIcon();
};

DECLARE_APP(ManagerApp)