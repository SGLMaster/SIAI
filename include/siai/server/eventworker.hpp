#pragma once

//event based worker reads signature and creates buffer for incoming data.
//When part of data arrives this worker resends it as soon as possible.
class EventWorker : public wxEvtHandler
{
public:
    EventWorker(wxSocketBase* pSock);
    virtual ~EventWorker();

protected:
    // outputs log message with IP and TCP port number prepended
    void LogWorker(const wxString& msg, wxLogLevel level = wxLOG_Info)
    {
        wxLogGeneric(level,
                     "%s:%d %s", m_peer.IPAddress(), m_peer.Service(), msg);
    }

    wxIPV4address m_peer;

private:
    wxSocketBase* m_socket;

    unsigned char m_signature[2];
    std::string m_inBuffer;
    int m_size;
    std::string m_outBuffer;
    int m_written;

    void OnSocketEvent(wxSocketEvent& pEvent);
    void DoWrite();
    void DoRead();

    wxDECLARE_EVENT_TABLE();
};