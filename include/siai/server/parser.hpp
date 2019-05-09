#pragma once

#include <wx/cmdline.h>

#include <memory>

class SIAIMap;
class DbConnector;
class DbConnectionOptions;

using DbConnectorPtr = std::unique_ptr<DbConnector>;
using MapPtr = std::unique_ptr<SIAIMap>;

namespace Parser
{
    static const wxCmdLineEntryDesc cmdLineDesc[] = 
    {
        { wxCMD_LINE_SWITCH, "h", "help", "No hay ayuda hasta ahora!", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
        { wxCMD_LINE_SWITCH, "c", "configure", "Inicia el asistente de configuracion." },
        { wxCMD_LINE_NONE }
    };

    void checkInput(wxCmdLineParser& parser, DbConnectorPtr& dbConnector, MapPtr& mapControl);
    void runOption(wxCmdLineParser& parser, DbConnectorPtr& dbConnector, MapPtr& mapControl);

    void tryToConnectDb(DbConnectorPtr& dbConnector, const DbConnectionOptions& options);
    void assertDbConnected(DbConnectorPtr& dbConnector);
}