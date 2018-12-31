#include <wx/splash.h>

#include "editor/mapeditorapp.hpp"
#include "editor/forms/mapeditorframe.hpp"

IMPLEMENT_APP(MapEditorApp)

bool MapEditorApp::OnInit()
{
    createAndShowSplashScreen();
    initializeFrame();

    return true;
}

void MapEditorApp::createAndShowSplashScreen()
{
    wxBitmap splashScreenBitmap;
    if(splashScreenBitmap.LoadFile("resources/splash.bmp", wxBITMAP_TYPE_BMP))
    {
        wxSplashScreen* splashScreen = new wxSplashScreen(  splashScreenBitmap,
                                                            wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
                                                            6000, NULL, -1, wxDefaultPosition, wxDefaultSize,
                                                            wxBORDER_SIMPLE|wxSTAY_ON_TOP);
        splashScreen->Show();
    }
}

void MapEditorApp::initializeFrame()
{
    m_frame = new MapEditorFrame( (wxFrame *)NULL );
    loadAndSetIcon();
    m_frame->Show();
}

void MapEditorApp::loadAndSetIcon()
{
    wxImage::AddHandler(new wxICOHandler);

    wxIcon testIconBitmap;
    testIconBitmap.LoadFile("resources/siai-icon.ico", wxBITMAP_TYPE_ICO);

    m_frame->SetIcon(testIconBitmap);
}
