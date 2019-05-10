#pragma once

#include "server/event.hpp"

class EventWorker;

WX_DECLARE_LIST(EventWorker, EList);

//main server class contains listening socket
//and list of two type worker classes that serve clients
class ServerApp : public wxApp
{
    wxDECLARE_EVENT_TABLE();
public:
    ServerApp() {}
    ~ServerApp() {}
private:
    virtual bool OnInit() override;
    virtual int OnExit() override;

    virtual void OnInitCmdLine(wxCmdLineParser& pParser) override;
    virtual bool OnCmdLineParsed(wxCmdLineParser& pParser);

    void OnSocketEvent(wxSocketEvent& pEvent);
    void OnWorkerEvent(WorkerEvent& pEvent);

    EList m_eventWorkers;
    wxSocketServer* m_listeningSocket;

    unsigned short m_tcpPort;
};

DECLARE_APP(ServerApp);