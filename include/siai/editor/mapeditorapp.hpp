#pragma once

#include <wx/app.h>

class MapEditorFrame;

class MapEditorApp : public wxApp
{
private:
    MapEditorFrame* m_frame;

    virtual bool OnInit() override;

    void createAndShowSplashScreen();
    void initializeFrame();
    void loadAndSetIcon();
};
