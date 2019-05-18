#include <wx/textfile.h>

#include "database/database.hpp"

#include "util/database.hpp"
#include "util/string.hpp"

void Util::Db::saveDbOptionsToFile(const DbConnectionOptions& options, const std::string& filename)
{
    wxTextFile configFile(filename);

    configFile.Open();

    configFile.Clear();

    configFile.AddLine(wxString("host = ") + wxString(options.host));
    configFile.AddLine(wxString("port = ") + wxString(std::to_string(options.port)) );
    configFile.AddLine(wxString("username = ") + wxString(options.user));
    configFile.AddLine(wxString("password = ") + wxString(options.password));

    configFile.Write();
    configFile.Close();
}

DbConnectionOptions Util::Db::loadDbOptionsFromFile(const std::string& filename)
{
    wxTextFile configFile(filename);

    configFile.Open();

    DbConnectionOptions tmpOptions;

    wxString curLine;
    std::string curLineStr;
    for(unsigned int i = 0; i < configFile.GetLineCount(); ++i)
    {
        curLine = configFile.GetLine(i);
        curLineStr = curLine.ToStdString();

        using namespace Util;
        if(curLine.StartsWith("host"))
            tmpOptions.host = String::getOptValueAsStr(curLineStr);

        else if(curLine.StartsWith("port"))
        {
            tmpOptions.port = static_cast<unsigned int>(String::getOptValueAsInt(curLineStr));
        }

        else if(curLine.StartsWith("username"))
            tmpOptions.user = String::getOptValueAsStr(curLineStr);

        else if(curLine.StartsWith("password"))
            tmpOptions.password = String::getOptValueAsStr(curLineStr);
    }

    configFile.Close();

    return tmpOptions;
}