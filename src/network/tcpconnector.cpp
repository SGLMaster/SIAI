#include "network/tcpconnector.hpp"

TcpConnector::TcpConnector(int serverPort, int socketPort, IMapEntity* parent) : wxEvtHandler(), 
                                                                                m_serverPort{serverPort},
                                                                                m_socketPort{socketPort}, 
                                                                                m_parent{parent}
{

}

TcpConnector::~TcpConnector()
{
    
}

void TcpConnector::open()
{

}

void TcpConnector::close()
{

}

bool TcpConnector::isConnected()
{

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