#include <wx/wx.h>
#include <wx/socket.h>
#include <wx/event.h>
#include <wx/cmdline.h>
#include <wx/app.h>

#include "server/event.hpp"
#include "server/eventworker.hpp"
#include "server/control.hpp"
#include "server/app.hpp"

#include "map/entities/agv.hpp"

#include "log.hpp"

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

EventWorker::EventWorker(wxSocketBase* pSock, ServerControl* serverControl) : 
            m_socket{pSock}, m_serverControl{serverControl}
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

void EventWorker::LogWorker(const wxString& msg, wxLogLevel level)
{
    // outputs log message with IP and TCP port number prepended
    wxLogGeneric(level, "%s:%d %s", m_peer.IPAddress(), m_peer.Service(), msg);
}

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
            LogWorker("Evento inesperado wxSOCKET_CONNECTION.", wxLOG_Error);
            break;

        case wxSOCKET_LOST:
            {
                LogWorker("Desconectado.");
                WorkerEvent e(this);
                e.m_workerFailed = m_written != m_size;
                wxGetApp().AddPendingEvent(e);

                m_agv.reset();
            }
            break;
    }
}

void EventWorker::DoRead()
{
    m_inBuffer.clear();
    char c;
    
    do
    {
        m_socket->Read(&c, 1);
        if(c == '\n')
            break;
        m_inBuffer += c;

        assertSocketError();
    }
    while(!m_socket->Error());
    
    m_socket->Discard();

    //LogWorker(wxString::Format("Mensaje en Socket: %s", std::string(m_inBuffer)));

    if(!m_agv)
    {
        m_agv = m_serverControl->processConnection(m_inBuffer);

        if(m_agv)
        {
            LogWorker(wxString::Format("Conexion relacionada con AGV#%d", m_agv->getId()));
            m_outBuffer = "OK";
        }
        else
            m_outBuffer = "ERROR";
    }
    else
    {
        m_outBuffer = m_serverControl->processCommand(m_agv, m_inBuffer);
    }

    DoWrite();
};

void  EventWorker::DoWrite()
{
    do
    {
        m_socket->Write(m_outBuffer.c_str(), m_outBuffer.size());
        if (m_socket->Error())
        {
            if (m_socket->LastError() != wxSOCKET_WOULDBLOCK)
            {
                LogWorker(wxString::Format( "Error de Escritura (%d): %s", m_socket->LastError(), 
                                            GetSocketErrorMsg(m_socket->LastError()) ) ,wxLOG_Error);
                m_socket->Close();
            }
            else
            {
                LogWorker("Escribir ahora bloquearia el servidor, esperando por evento OUTPUT...");
            }
        }
        else
        {
            m_outBuffer.clear();
            break;
        }
    }
    while (!m_socket->Error());
}

void EventWorker::assertSocketError()
{
    if (m_socket->Error())
    {
        if (m_socket->LastError() != wxSOCKET_WOULDBLOCK)
        {
            LogWorker(wxString::Format("Error en Lectura (%d): %s", m_socket->LastError(), 
                            GetSocketErrorMsg(m_socket->LastError())), wxLOG_Error);

            m_socket->Close();
        }
    }
}

wxBEGIN_EVENT_TABLE(EventWorker,wxEvtHandler)
    EVT_SOCKET(wxID_ANY,EventWorker::OnSocketEvent)
wxEND_EVENT_TABLE()