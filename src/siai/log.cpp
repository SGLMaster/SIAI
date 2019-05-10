#include <wx/msgdlg.h>
#include <wx/datetime.h>

#include "log.hpp"

void Log::simpleMessage(const std::string& msg, bool logToCmd)
{
    if(logToCmd)
        wxPrintf(_("%s\n"), _(msg));
    else
        wxMessageBox(_(msg), "Log");
}

void Log::timestampMessage(const std::string& msg, bool logToCmd)
{
    wxDateTime curTime = wxDateTime::GetTimeNow();
    unsigned short curHour = curTime.GetHour();
    unsigned short curMinute = curTime.GetMinute();
    unsigned short curSecond = curTime.GetSecond();

    if(logToCmd)
        wxPrintf( wxT("[%.2u:%.2u:%.2u] %s\n"), curHour, curMinute, curSecond, _(msg) );
    else
        wxMessageBox(_(msg), "Log");
}

void Log::warning(const std::string& msg, bool logToCmd)
{
    if(logToCmd)
        wxPrintf(_("Advertencia: %s\n"), _(msg));
    else
        wxMessageBox(_(msg), _("Advertencia"), wxICON_WARNING);
}

void Log::fatalError(const std::string& msg, bool logToCmd)
{
    if(logToCmd)
        wxPrintf(_("Error Fatal: %s\n"), _(msg));
    else
        wxMessageBox(_(msg), _("Error Fatal"), wxICON_ERROR);
}
