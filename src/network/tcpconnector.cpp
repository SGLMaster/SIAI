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

void TcpConnector::Open()
{

}

void TcpConnector::Close()
{

}

bool TcpConnector::IsConnected()
{

}

void TcpConnector::Write(const std::string& msg)
{

}

void TcpConnector::OnServerEvent( wxSocketEvent& event )
{

}

void TcpConnector::OnSocketEvent( wxSocketEvent& event )
{


}