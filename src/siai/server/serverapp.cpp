#include <wx/wxprec.h>

#include <wx/app.h>
#include <wx/cmdline.h>

#include "database/database.hpp"

#include "server/parser.hpp"

#define CONSOLE_APP
#include "map/siaimap.hpp"

#include <string>

using DbConnectorPtr = std::unique_ptr<DbConnector>;
using MapPtr = std::unique_ptr<SIAIMap>;

void noArgsMessage(int argc);

int main(int argc, char **argv)
{
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");

    wxInitializer initializer;
    if(!initializer)
    {
        fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }

    DbConnectorPtr mainDbConnectorPtr;
    MapPtr mainMapPtr;

    wxCmdLineParser parser(Parser::cmdLineDesc, argc, argv);
    Parser::checkInput(parser, mainDbConnectorPtr, mainMapPtr);

    noArgsMessage(argc);

    return 0;
}

void noArgsMessage(int argc)
{
    if(argc == 1)
    {
        // Run a message if there were no arguments
        wxPrintf("Welcome to the SIAI Server program!\n");
        wxPrintf("For more information, run it again with the --help option\n");
    }
}