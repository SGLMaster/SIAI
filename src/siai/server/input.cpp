#include <wx/cmdline.h>

#include "server/input.hpp"

std::string CmdInput::getString(const std::string& name)
{
    wxChar input[128];
    wxPrintf("%s: ", wxString(name));
    if ( !wxFgets(input, WXSIZEOF(input), stdin) )
        return std::string("");

    // kill the last '\n'
    input[wxStrlen(input) - 1] = 0;

    return wxString(input).ToStdString();
}

int CmdInput::getUInt(const std::string& name)
{
    wxChar input[128];
    wxPrintf("%s: ", wxString(name));
    if ( !wxFgets(input, WXSIZEOF(input), stdin) )
        return 0;

    // kill the last '\n'
    input[wxStrlen(input) - 1] = 0;

    long value;
    if (!wxString(input).ToLong(&value))
        return 0;

    return static_cast<unsigned int>(value);
}