#include "log.hpp"

#include <wx/msgdlg.h>

void Log::msgBox(const std::string& msg)
{
    wxMessageBox(msg, "Log");
}
