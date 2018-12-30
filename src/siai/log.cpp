#include "log.hpp"

#include <wx/msgdlg.h>

void Log::simpleMessage(const std::string& msg)
{
    wxMessageBox(_(msg), "Log");
}

void Log::warning(const std::string& msg)
{
    wxMessageBox(_(msg), _("Advertencia"), wxICON_WARNING);
}

void Log::fatalError(const std::string& msg)
{
    wxMessageBox(_(msg), _("Error Fatal"), wxICON_ERROR);
}
