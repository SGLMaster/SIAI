#include <wx/app.h>
#include <wx/cmdline.h>

#include "database/database.hpp"

#define CONSOLE_APP
#include "map/siaimap.hpp"

#include "server/serverapp.hpp"

#include "server/server.hpp"

IMPLEMENT_APP_CONSOLE(ServerApp);

bool ServerApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    return true;
}

int ServerApp::OnRun()
{
    return wxApp::OnRun();
}

int ServerApp::OnExit()
{
    return 0;
}

void ServerApp::OnInitCmdLine(wxCmdLineParser& pParser)
{
    wxApp::OnInitCmdLine(pParser);

    pParser.AddSwitch(wxT("c"), wxT("configure"), _("Configure the server."), wxCMD_LINE_PARAM_OPTIONAL);
}

bool ServerApp::OnCmdLineParsed(wxCmdLineParser& pParser)
{
    if (pParser.Found(wxT("c")))
    {
        DbConnectorPtr mainDbConnectorPtr;
        MapPtr mainMapPtr;

        Server::configure(mainDbConnectorPtr, mainMapPtr);
    }
    else
        noValidOptionMessage();

    return wxApp::OnCmdLineParsed(pParser);
};

void ServerApp::noValidOptionMessage() const
{
    wxPrintf("You need to run the program with a valid option!\n");
    wxPrintf("For more information, run it again with the --help option\n");
    exit(0);
}