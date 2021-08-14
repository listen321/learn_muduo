#ifndef __LL_TCP_SERVER_H__
#define __LL_TCP_SERVER_H__

#include "base/Types.h"
#include "base/ThreadPool.h"
#include "net/InetAddress.h"
#include "net/Socket.h"
#include "net/Epoll.h"
#include "net/EventLoopM.h"
#include "net/TcpConnect.h"
#include "EventLoop.h"
#include <memory>
#include <netinet/in.h>

namespace LL
{
class TcpServer
{
    public:
        TcpServer(uint16_t port, size_t thread_size = 4, in_addr_t in_addr = INADDR_ANY);
        TcpServer(const std::string& ip, uint16_t port, size_t thread_size = 4);
        ~TcpServer();

        void start();
        void stop();

        EventLoop* getNextLoop();

        void addConnect();
        void removeConnect(uint64_t id);

        static uint64_t getTcpConnectId();
        using TcpConnHashMap = std::unordered_map<uint64_t, TcpConnect>;
    private:
        InetAddress    _addr;
        Socket         _socket;
        EventLoop      _accept_pool;
        ThreadPool     _thread_pool;
        EventLoopM     _evenloop_m;
        TcpConnHashMap _conn_m;
};
}

#endif
