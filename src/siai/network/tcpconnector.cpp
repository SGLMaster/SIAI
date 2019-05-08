#include "network/tcpconnector.hpp"

#include "log.hpp"

TcpConnector::TcpConnector(int serverPort, int socketPort, IMapEntity* parent) : wxEvtHandler(), 
                                                                                m_serverPort{serverPort},
                                                                                m_socketPort{socketPort}, 
                                                                                m_parent{parent}
{

}

TcpConnector::~TcpConnector()
{
    close();
}

void TcpConnector::open()
{
    //Create and set address to 0.0.0.0:m_serverPort
    wxIPV4address address;
    address.Service(m_serverPort);

    Log::simpleMessage( std::string("Creando servidor en ") + address.IPAddress().ToStdString() + ":" 
                        + std::to_string(address.Service()) );

    m_server = std::make_unique<wxSocketServer>(address);

    if(!m_server->IsOk())
    {
        Log::warning("No se puede usar el puerto especificado!");
        return;
    }

    wxIPV4address realAddress;
    if ( !m_server->GetLocal(realAddress) )
    {
        Log::warning("Error: No se pudo obtener la direccion!");
        return;
    }
    else
    {
        Log::warning( std::string("Servidor escuchando en %s:%u") + realAddress.IPAddress().ToStdString() 
                    + std::to_string(realAddress.Service()) );
    }

    //Setup the event handler and subscribe to connection events
    m_server->SetEventHandler(*this, m_serverPort);
    m_server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_server->Notify(true);

    //Connect the events to a handler function
    Bind(wxEVT_SOCKET, &TcpConnector::OnServerEvent, this, m_serverPort);
}

void TcpConnector::close()
{
    if(m_socket)
    {
        m_socket->Destroy();
        m_socket.release();
    }
    if(m_server)
    {
        m_server->Destroy();
        m_server.release();
    }
}

bool TcpConnector::isConnected()
{
    if(m_server)
    {
        if(m_socket)
            return m_socket->IsConnected();
    }

    return false;
}

void TcpConnector::write(const std::string& msg)
{

}

void TcpConnector::OnServerEvent( wxSocketEvent& event )
{

}

void TcpConnector::OnSocketEvent( wxSocketEvent& event )
{


}