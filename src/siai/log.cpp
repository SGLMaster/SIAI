#include <wx/msgdlg.h>

#include "log.hpp"

void Log::simpleMessage(const std::string& msg, bool logToCmd)
{
    if(logToCmd)
        wxPrintf(_("Mensaje: %s"), _(msg));
    else
        wxMessageBox(_(msg), "Log");
}

void Log::warning(const std::string& msg, bool logToCmd)
{
    if(logToCmd)
        wxPrintf(_("Advertencia: %s"), _(msg));
    else
        wxMessageBox(_(msg), _("Advertencia"), wxICON_WARNING);
}

void Log::fatalError(const std::string& msg, bool logToCmd)
{
    if(logToCmd)
        wxPrintf(_("Error Fatal: %s"), _(msg));
    else
        wxMessageBox(_(msg), _("Error Fatal"), wxICON_ERROR);
}
