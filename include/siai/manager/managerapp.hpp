#pragma once

#include <wx/app.h>

class ManagerFrame;

class ManagerApp : public wxApp
{
private:
    ManagerFrame* m_frame;

    virtual bool OnInit() override;

    void createAndShowSplashScreen();
    void initializeFrame();
    void loadAndSetIcon();
};
