#include <wx/splash.h>

#include "manager/managerapp.hpp"
#include "manager/forms/managerframe.hpp"

IMPLEMENT_APP(ManagerApp)

ManagerApp::ManagerApp() : m_updateMapThread{NULL}, m_shuttingDown{false} {}

ManagerApp::~ManagerApp() = default;

bool ManagerApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    createAndShowSplashScreen();
    initializeFrame();

    return true;
}

void ManagerApp::createAndShowSplashScreen()
{
    wxBitmap splashScreenBitmap;
    if(splashScreenBitmap.LoadFile("resources/splash.bmp", wxBITMAP_TYPE_BMP))
    {
        wxSplashScreen* splashScreen = new wxSplashScreen(  splashScreenBitmap,
                                                            wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
                                                            3000, NULL, -1, wxDefaultPosition, wxDefaultSize,
                                                            wxBORDER_SIMPLE|wxSTAY_ON_TOP);
        splashScreen->Show();
    }
}

void ManagerApp::initializeFrame()
{
    m_frame = new ManagerFrame( (wxFrame *)NULL );
    loadAndSetIcon();
    m_frame->Show();
}

void ManagerApp::loadAndSetIcon()
{
    wxImage::AddHandler(new wxICOHandler);

    wxIcon testIconBitmap;
    testIconBitmap.LoadFile("resources/siai-icon.ico", wxBITMAP_TYPE_ICO);

    m_frame->SetIcon(testIconBitmap);
}
