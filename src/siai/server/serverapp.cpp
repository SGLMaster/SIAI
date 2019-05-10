#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/event.h>
#include <wx/list.h>
#include <wx/cmdline.h>
#include <wx/datetime.h>
#include <wx/thread.h>

#include "server/serverapp.hpp"
#include "server/eventworker.hpp"
#include "server/control.hpp"

IMPLEMENT_APP_CONSOLE(ServerApp)

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(EList);

void ServerApp::OnInitCmdLine(wxCmdLineParser& pParser)
{
    wxApp::OnInitCmdLine(pParser);
    pParser.AddSwitch("c", "configure", "Configure server.");
    pParser.AddOption("p", "port", "listen on given port (default 3000)", wxCMD_LINE_VAL_NUMBER);
}

bool ServerApp::OnCmdLineParsed(wxCmdLineParser& pParser)
{
    if (pParser.Found("c"))
    {
        m_serverControl->configure();
    }

    long port;
    if (pParser.Found("p", &port))
    {
        if ( port <= 0 || port > USHRT_MAX )
        {
            wxLogError("Invalid port number %ld, must be in 0..%u range.", port, USHRT_MAX);
            return false;
        }

        m_tcpPort = static_cast<unsigned short>(port);
        wxLogMessage("Will listen on port %u", m_tcpPort);
    }

    return wxApp::OnCmdLineParsed(pParser);
}

bool ServerApp::OnInit()
{
    wxLog* logger = new wxLogStderr();
    wxLog::SetActiveTarget(logger);

    m_serverControl = std::make_unique<ServerControl>();

    m_tcpPort = 3000;

    if (!wxApp::OnInit())
        return false;

    wxIPV4address address;
    address.Service(m_tcpPort);

    m_listeningSocket = new wxSocketServer(address, wxSOCKET_NOWAIT|wxSOCKET_REUSEADDR);
    m_listeningSocket->SetEventHandler(*this);
    m_listeningSocket->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_listeningSocket->Notify(true);

    if (!m_listeningSocket->IsOk())
    {
        wxLogError("Cannot bind listening socket");
        return false;
    }

    wxLogMessage("Server listening at port %u, waiting for connections", m_tcpPort);
    return true;
}


int ServerApp::OnExit()
{
    for(EList::compatibility_iterator it2 = m_eventWorkers.GetFirst(); it2; it2->GetNext())
    {
        delete it2->GetData();
    }

    m_eventWorkers.Clear();
    m_listeningSocket->Destroy();
    return 0;
}

void ServerApp::OnSocketEvent(wxSocketEvent& pEvent)
{
    switch(pEvent.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
            wxLogError("Unexpected wxSOCKET_INPUT in wxSocketServer");
            break;
        case wxSOCKET_OUTPUT:
            wxLogError("Unexpected wxSOCKET_OUTPUT in wxSocketServer");
        break;
        case wxSOCKET_CONNECTION:
        {
            wxSocketBase* sock = m_listeningSocket->Accept();
            wxIPV4address addr;
            if (!sock->GetPeer(addr))
            {
                wxLogError("Server: cannot get peer info");
            } else {
                wxLogMessage("Got connection from %s:%d",addr.IPAddress().c_str(), addr.Service());
            }

            EventWorker* w = new EventWorker(sock);
            m_eventWorkers.Append(w);
        }
        break;
        case wxSOCKET_LOST:
            wxLogError("Unexpected wxSOCKET_LOST in wxSocketServer");
        break;
    }
}

void ServerApp::OnWorkerEvent(WorkerEvent& pEvent)
{
    for(EList::compatibility_iterator it2 = m_eventWorkers.GetFirst(); it2 ; it2 = it2->GetNext())
    {
        if (it2->GetData() == pEvent.m_sender)
        {
            wxLogVerbose("Deleting event worker(%lu left)", static_cast<unsigned long>( m_eventWorkers.GetCount() ));
            delete it2->GetData();
            m_eventWorkers.DeleteNode(it2);
            break;
        }
    }
}

wxBEGIN_EVENT_TABLE(ServerApp,wxEvtHandler)
  EVT_SOCKET(wxID_ANY,ServerApp::OnSocketEvent)
  EVT_WORKER(ServerApp::OnWorkerEvent)
wxEND_EVENT_TABLE()