/*
#include <wx/app.h>
#include <wx/cmdline.h>

#include "database/database.hpp"

#define CONSOLE_APP
#include "map/siaimap.hpp"

#include "server/serverapp.hpp"

#include "server/server.hpp"

IMPLEMENT_APP_CONSOLE(ServerApp);

bool ServerApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    return true;
}

int ServerApp::OnRun()
{
    return wxApp::OnRun();
}

int ServerApp::OnExit()
{
    return 0;
}

void ServerApp::OnInitCmdLine(wxCmdLineParser& pParser)
{
    wxApp::OnInitCmdLine(pParser);

    pParser.AddSwitch(wxT("c"), wxT("configure"), _("Configure the server."), wxCMD_LINE_PARAM_OPTIONAL);
}

bool ServerApp::OnCmdLineParsed(wxCmdLineParser& pParser)
{
    if (pParser.Found(wxT("c")))
    {
        DbConnectorPtr mainDbConnectorPtr;
        MapPtr mainMapPtr;

        Server::configure(mainDbConnectorPtr, mainMapPtr);
    }
    else
        noValidOptionMessage();

    return wxApp::OnCmdLineParsed(pParser);
};

void ServerApp::noValidOptionMessage() const
{
    wxPrintf("You need to run the program with a valid option!\n");
    wxPrintf("For more information, run it again with the --help option\n");
    exit(0);
}
*/

#include "wx/wx.h"
#include "wx/socket.h"
#include "wx/event.h"
#include "wx/list.h"
#include "wx/cmdline.h"
#include "wx/datetime.h"
#include "wx/thread.h"

const char *GetSocketErrorMsg(int pSockError)
{
    switch(pSockError)
    {
        case wxSOCKET_NOERROR:
            return "wxSOCKET_NOERROR";

        case wxSOCKET_INVOP:
            return "wxSOCKET_INVOP";

        case wxSOCKET_IOERR:
            return "wxSOCKET_IOERR";

        case wxSOCKET_INVADDR:
            return "wxSOCKET_INVADDR";

        case wxSOCKET_NOHOST:
            return "wxSOCKET_NOHOST";

        case wxSOCKET_INVPORT:
            return "wxSOCKET_INVPORT";

        case wxSOCKET_WOULDBLOCK:
            return "wxSOCKET_WOULDBLOCK";

        case wxSOCKET_TIMEDOUT:
            return "wxSOCKET_TIMEDOUT";

        case wxSOCKET_MEMERR:
            return "wxSOCKET_MEMERR";

        default:
            return "Unknown";
    }
}

//event sent by workers to server class
//after client is served
const wxEventType wxEVT_WORKER = wxNewEventType();
#define EVT_WORKER(func) DECLARE_EVENT_TABLE_ENTRY( wxEVT_WORKER, -1, -1, (wxObjectEventFunction) (wxEventFunction) (WorkerEventFunction) & func, (wxObject *) NULL ),

class WorkerEvent : public wxEvent
{
public:
    WorkerEvent(void* pSender)
    {
        SetId(-1);
        SetEventType(wxEVT_WORKER);
        m_sender = pSender;
        m_exit = false;
        m_workerFailed = false;
    }

    virtual wxEvent* Clone() const
    {
        return new WorkerEvent(*this);
    }

    void* m_sender;
    bool m_exit;
    bool m_workerFailed;
};

typedef void (wxEvtHandler::*WorkerEventFunction)(WorkerEvent&);

class EventWorker;

WX_DECLARE_LIST(EventWorker, EList);

//main server class contains listening socket
//and list of two type worker classes that serve clients
class Server : public wxApp
{
    wxDECLARE_EVENT_TABLE();
public:
    Server() : m_maxConnections(-1) {}
    ~Server() {}
private:
    virtual bool OnInit();
    virtual int OnExit();

    void OnInitCmdLine(wxCmdLineParser& pParser);
    bool OnCmdLineParsed(wxCmdLineParser& pParser);

    void OnSocketEvent(wxSocketEvent& pEvent);
    void OnWorkerEvent(WorkerEvent& pEvent);

    EList m_eventWorkers;
    wxSocketServer* m_listeningSocket;

    // statistics
    unsigned m_eventWorkersCreated;
    unsigned m_eventWorkersDone;
    unsigned m_eventWorkersFailed;
    unsigned m_maxEventWorkers;

    long int m_maxConnections;

    unsigned short m_port;

    wxTimer mTimer;
};

DECLARE_APP(Server);

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

IMPLEMENT_APP_CONSOLE(Server)

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(EList);

void
Server::OnInitCmdLine(wxCmdLineParser& pParser)
{
    wxApp::OnInitCmdLine(pParser);
    pParser.AddSwitch("e","events", "Use event based workers only");
    pParser.AddOption("m","max", "Exit after <n> connections", wxCMD_LINE_VAL_NUMBER);
    pParser.AddOption("p","port", "listen on given port (default 3000)", wxCMD_LINE_VAL_NUMBER);
}

bool Server::OnCmdLineParsed(wxCmdLineParser& pParser)
{
    if (pParser.Found("m",&m_maxConnections))
    {
        wxLogMessage("%ld connection(s) to exit",m_maxConnections);
    }

    long port;
    if (pParser.Found("p", &port))
    {
        if ( port <= 0 || port > USHRT_MAX )
        {
            wxLogError("Invalid port number %ld, must be in 0..%u range.",
                       port, USHRT_MAX);
            return false;
        }

        m_port = static_cast<unsigned short>(port);
        wxLogMessage("Will listen on port %u", m_port);
    }

    return wxApp::OnCmdLineParsed(pParser);
}

bool Server::OnInit()
{
    wxLog* logger = new wxLogStderr();
    wxLog::SetActiveTarget(logger);

    m_port = 3000;

    //send interesting things to console
    if (!wxApp::OnInit())
        return false;

    //setup listening socket
    wxIPV4address la;
    la.Service(m_port);
    m_listeningSocket = new wxSocketServer(la,wxSOCKET_NOWAIT|wxSOCKET_REUSEADDR);
    m_listeningSocket->SetEventHandler(*this);
    m_listeningSocket->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_listeningSocket->Notify(true);
    if (!m_listeningSocket->IsOk())
    {
        wxLogError("Cannot bind listening socket");
        return false;
    }

    m_eventWorkersCreated = 0;
    m_eventWorkersDone = 0;
    m_eventWorkersFailed = 0;
    m_maxEventWorkers = 0;

    wxLogMessage("Server listening at port %u, waiting for connections", m_port);
    return true;
}


int Server::OnExit()
{
    for(EList::compatibility_iterator it2 = m_eventWorkers.GetFirst(); it2; it2->GetNext())
    {
        delete it2->GetData();
    }

    m_eventWorkers.Clear();
    m_listeningSocket->Destroy();
    return 0;
}

void Server::OnSocketEvent(wxSocketEvent& pEvent)
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
            if (m_eventWorkers.GetCount() > m_maxEventWorkers)
                m_maxEventWorkers++;
            m_eventWorkersCreated++;
        }
        break;
        case wxSOCKET_LOST:
            wxLogError("Unexpected wxSOCKET_LOST in wxSocketServer");
        break;
    }
}

void Server::OnWorkerEvent(WorkerEvent& pEvent)
{
    //wxLogMessage("Got worker event");
    for(EList::compatibility_iterator it2 = m_eventWorkers.GetFirst(); it2 ; it2 = it2->GetNext())
    {
        if (it2->GetData() == pEvent.m_sender)
        {
            wxLogVerbose("Deleting event worker (%lu left)",
                         static_cast<unsigned long>( m_eventWorkers.GetCount() ));
            delete it2->GetData();
            m_eventWorkers.DeleteNode(it2);
            if (!pEvent.m_workerFailed)
                m_eventWorkersDone++;
            else
                m_eventWorkersFailed++;
            break;
        }
    }

    if (m_eventWorkers.GetCount() == 0)
    {
        mTimer.Start(1000,true);
    }
}

wxBEGIN_EVENT_TABLE(Server,wxEvtHandler)
  EVT_SOCKET(wxID_ANY,Server::OnSocketEvent)
  EVT_WORKER(Server::OnWorkerEvent)
wxEND_EVENT_TABLE()

EventWorker::EventWorker(wxSocketBase* pSock) : m_socket(pSock)
{
    m_socket->SetNotify(wxSOCKET_LOST_FLAG|wxSOCKET_INPUT_FLAG|wxSOCKET_OUTPUT_FLAG);
    m_socket->Notify(true);
    m_socket->SetEventHandler(*this);
    m_socket->SetFlags(wxSOCKET_NOWAIT);
    m_socket->GetPeer(m_peer);
}

EventWorker::~EventWorker()
{
    m_socket->Destroy();
}

void EventWorker::DoRead()
{
    m_inBuffer.clear();
    char c;

    do
    {
        m_socket->Read(&c, 1);
        if(c == '\r')
            break;
        m_inBuffer += c;
    }
    while(!m_socket->Error());

    LogWorker(wxString(std::string("Mensaje en Socket: ") + m_inBuffer));

    //Discard every character after '\r' we don't need to hold a buffer
    m_socket->Discard();
};

void EventWorker::OnSocketEvent(wxSocketEvent& pEvent)
{
    switch(pEvent.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
            DoRead();
            break;

        case wxSOCKET_OUTPUT:
            if(m_outBuffer.length() > 0)
                DoWrite();
            break;

        case wxSOCKET_CONNECTION:
            LogWorker("Unexpected wxSOCKET_CONNECTION in EventWorker", wxLOG_Error);
            break;

        case wxSOCKET_LOST:
            {
                LogWorker("Connection lost");
                WorkerEvent e(this);
                e.m_workerFailed = m_written != m_size;
                wxGetApp().AddPendingEvent(e);
            }
            break;
    }
}

void  EventWorker::DoWrite()
{
    
}

wxBEGIN_EVENT_TABLE(EventWorker,wxEvtHandler)
    EVT_SOCKET(wxID_ANY,EventWorker::OnSocketEvent)
wxEND_EVENT_TABLE()