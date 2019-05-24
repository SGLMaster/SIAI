#pragma once

#include "map/entities/entities.hpp"

class ServerControl;

//event based worker reads signature and creates buffer for incoming data.
//When part of data arrives this worker resends it as soon as possible.
class EventWorker : public wxEvtHandler
{
private:
    wxSocketBase* m_socket;
    ServerControl* m_serverControl;

    unsigned char m_signature[2];
    std::string m_inBuffer;
    int m_size;
    std::string m_outBuffer;
    int m_written;

    Entities::AgvPtr m_agv;

protected:
    wxIPV4address m_peer;

public:
    EventWorker(wxSocketBase* pSock, ServerControl* serverControl);
    virtual ~EventWorker();

protected:
    void LogWorker(const wxString& msg, wxLogLevel level = wxLOG_Info);

private:
    void OnSocketEvent(wxSocketEvent& pEvent);

    void DoRead();
    void DoWrite();

    void assertSocketError();

    wxDECLARE_EVENT_TABLE();
};