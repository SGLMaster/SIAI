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
        IMapEntity* m_parent;

        std::string m_lastMsg;

        std::unique_ptr<wxSocketServer> m_server;
        std::unique_ptr<wxSocketBase> m_socket;

        void OnServerEvent( wxSocketEvent& event );
        void OnSocketEvent( wxSocketEvent& event );

        static const int SERVER_ID_OFFSET{20000};
        static const int SOCKET_ID_OFFSET{30000};

    public:
        TcpConnector(IMapEntity* parent);
        TcpConnector() = delete;
        virtual ~TcpConnector();

        //IConnector virtual methods
        virtual void Open() override;
        virtual void Close() override;
        virtual bool IsConnected() override;
        virtual void Write(const std::string& msg) override;
};
