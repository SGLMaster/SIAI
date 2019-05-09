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

    Log::simpleMessage(std::string("Creando servidor en ") + address.IPAddress().ToStdString() + ":" 
                        + std::to_string(address.Service()), true);

    m_server = std::make_unique<wxSocketServer>(address);

    if(!m_server->IsOk())
    {
        Log::warning("No se puede usar el puerto especificado!", true);
        return;
    }

    wxIPV4address realAddress;
    if ( !m_server->GetLocal(realAddress) )
    {
        Log::warning("Error: No se pudo obtener la direccion!", true);
        return;
    }
    else
    {
        Log::simpleMessage(std::string("Servidor escuchando en ") + realAddress.IPAddress().ToStdString() + ":" 
                    + std::to_string(realAddress.Service()), true);
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

void TcpConnector::OnServerEvent(wxSocketEvent& event)
{
    if(m_socket)
    {
        Log::fatalError("Puerto Ocupado", true);
        m_server->SetNotify(wxSOCKET_CONNECTION_FLAG);
        return;
    }

    std::unique_ptr<wxSocketBase> tmpUnique(m_server->Accept(false));
    m_socket = std::move(tmpUnique);

    if (m_socket)
    {
        wxIPV4address addr;
        if ( !m_socket->GetPeer(addr) )
        {
            Log::fatalError("Cliente desconocido, cerrando conexion.", true);
            m_socket->Destroy();
            m_socket.release();
            return;
        }
        else
        {
            //Setting the handler and the flags for the events
            //The handler ID is set to the same number as the socket's port number
            m_socket->SetEventHandler(*this, m_socketPort);
            m_socket->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
            m_socket->Notify(true);

            //Connecting to the handler method
            Bind(wxEVT_SOCKET, &TcpConnector::OnSocketEvent, this, m_socketPort);

            //Success
            Log::simpleMessage(std::string("Nueva conexion aceptada con cliente ") + addr.IPAddress().ToStdString() 
                                            + ":" + std::to_string(addr.Service()), true);
            Log::simpleMessage(std::string("Socket ID: ") + std::to_string(m_socketPort), true);

        }
    }
    else
    {
        Log::fatalError("Error: no se pudo aceptar la conexion.", true);
    }
}

void TcpConnector::OnSocketEvent( wxSocketEvent& event )
{


}