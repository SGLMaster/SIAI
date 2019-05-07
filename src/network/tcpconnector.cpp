#include <network/tcpconnector.hpp>

TcpConnector::TcpConnector(IMapEntity* parent) : wxEvtHandler(), m_parent{parent}
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

void TcpConnector::OnServerEvent( wxSocketEvent& event )
{

}

void TcpConnector::OnSocketEvent( wxSocketEvent& event )
{


}

bool TcpConnector::IsConnected()
{

}

void TcpConnector::Write(const std::string& msg)
{

}
