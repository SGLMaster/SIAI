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
    int m_portNumber;

    IMapEntity* m_parent;

    std::unique_ptr<wxSocketServer> m_server;
    std::unique_ptr<wxSocketBase> m_socket;

public:
    TcpConnector(int portNumber, IMapEntity* parent);
    TcpConnector() = delete;
    virtual ~TcpConnector();

    virtual void Open() override;
    virtual void Close() override;
    virtual bool IsConnected() override;
    virtual void Write(const std::string& msg) override;

private:
    void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
};
