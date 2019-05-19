#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/event.h>
#include <wx/list.h>
#include <wx/cmdline.h>
#include <wx/datetime.h>
#include <wx/thread.h>

#include "server/app.hpp"
#include "server/eventworker.hpp"
#include "server/control.hpp"

IMPLEMENT_APP_CONSOLE(ServerApp)

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(EList);

void ServerApp::OnInitCmdLine(wxCmdLineParser& pParser)
{
    wxApp::OnInitCmdLine(pParser);
    pParser.AddSwitch("c", "configure", "Configurar las opciones necesarias para correr el servidor.");
    pParser.AddSwitch("r", "run", "Iniciar el servidor con la configuracion guardada.");
    pParser.AddOption("p", "port", "Utilizar un puerto en especifico (por defecto 3000).", wxCMD_LINE_VAL_NUMBER);
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
            wxLogError("Numero de puerto invalido: %ld, debe estar dentro del rango 0..%u.", port, USHRT_MAX);
            return false;
        }

        m_tcpPort = static_cast<unsigned short>(port);
        wxLogMessage("Puerto a utilizar: %u", m_tcpPort);
    }

    if(pParser.Found("r"))
    {
        m_serverControl->init();

        wxIPV4address address;
        address.Service(m_tcpPort);

        m_listeningSocket = new wxSocketServer(address, wxSOCKET_NOWAIT|wxSOCKET_REUSEADDR);
        m_listeningSocket->SetEventHandler(*this);
        m_listeningSocket->SetNotify(wxSOCKET_CONNECTION_FLAG);
        m_listeningSocket->Notify(true);

        if (!m_listeningSocket->IsOk())
        {
            wxLogError("No se pudo crear el socket!");
            return false;
        }

        wxLogMessage("Servidor escuchando en puerto %u, esperando conexiones...", m_tcpPort);
    }
    else
    {
        exit(0);
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
            wxLogError("Evento inesperado wxSOCKET_INPUT en wxSocketServer");
            break;
        case wxSOCKET_OUTPUT:
            wxLogError("Evento inesperado wxSOCKET_OUTPUT en wxSocketServer");
        break;
        case wxSOCKET_CONNECTION:
        {
            wxSocketBase* sock = m_listeningSocket->Accept();
            wxIPV4address addr;
            if (!sock->GetPeer(addr))
            {
                wxLogError("Servidor: no se puede obtener información del host remoto");
            } else {
                wxLogMessage("Conexion establecida con %s:%d",addr.IPAddress().c_str(), addr.Service());
            }

            EventWorker* w = new EventWorker(sock, m_serverControl.get());
            m_eventWorkers.Append(w);
        }
        break;
        case wxSOCKET_LOST:
            wxLogError("Evento inesperado wxSOCKET_LOST en wxSocketServer");
        break;
    }
}

void ServerApp::OnWorkerEvent(WorkerEvent& pEvent)
{
    for(EList::compatibility_iterator it2 = m_eventWorkers.GetFirst(); it2 ; it2 = it2->GetNext())
    {
        if (it2->GetData() == pEvent.m_sender)
        {
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