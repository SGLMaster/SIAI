#include "network/tcpconnector.hpp"

TcpConnector::TcpConnector(int portNumber, IMapEntity* parent) : wxEvtHandler(), 
                                                                m_portNumber{portNumber}, m_parent{parent}
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