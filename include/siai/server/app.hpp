#pragma once

#include "server/event.hpp"

#include <memory>

class EventWorker;

WX_DECLARE_LIST(EventWorker, EList);

class ServerControl;

//main server class contains listening socket
//and list of two type worker classes that serve clients
class ServerApp : public wxApp
{
    wxDECLARE_EVENT_TABLE();
    
private:
    EList m_eventWorkers;
    wxSocketServer* m_listeningSocket;

    std::unique_ptr<ServerControl> m_serverControl;

    unsigned short m_tcpPort;

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
};

DECLARE_APP(ServerApp);