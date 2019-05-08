#pragma once

#include "network/connector.hpp"

#include <wx/event.h>
#include <wx/socket.h>

#include <memory>
#include <string>

class IMapEntity;

class TcpConnector : public IConnector, public wxEvtHandler
{
private:
    int m_serverPort;
    int m_socketPort;

    IMapEntity* m_parent;

    std::unique_ptr<wxSocketServer> m_server;
    std::unique_ptr<wxSocketBase> m_socket;

public:
    TcpConnector(int serverPort, int socketPort, IMapEntity* parent);
    TcpConnector() = delete;
    virtual ~TcpConnector();

    virtual void open() override;
    virtual void close() override;
    virtual bool isConnected() override;
    virtual void write(const std::string& msg) override;

private:
    void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
};
