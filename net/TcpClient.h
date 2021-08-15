#ifndef __LL_TCP_CLIENT_H__
#define __LL_TCP_CLIENT_H__

#include "net/InetAddress.h"
#include "net/Buffer.h"
#include "net/Epoll.h"
#include "net/EventLoop.h"
#include "net/TcpConnect.h"

namespace LL
{

class TcpClient
{
    public:
        TcpClient(const string& ip, uint16_t port);
        ~TcpClient();

        bool connectServer();
        void start();
        int getFd() { return _fd; }

        void sendMsg(const std::string& s);
        void peerClose();
        void shutdownConnect();
        void closeSocket();

        void setRecvCallback(RecvCallback call)
        {
            _call = call;
        }
    private:
        void _closeSocket();
        int                         _fd = -1;
        std::unique_ptr<TcpConnect> _tcp_connect;
        InetAddress                 _server_addr;
        EventLoop                   _event_loop;
        RecvCallback                _call;
};

}

#endif
