#include "TcpServer.h"
#include "base/LoggingThread.h"

namespace LL
{

TcpServer::TcpServer(uint16_t port, size_t thread_size, in_addr_t in_addr)
    :_addr(port, in_addr)
     ,_accept_pool(this)
     ,_thread_pool(thread_size)
     ,_evenloop_m(thread_size, this)
{
}

TcpServer::TcpServer(const std::string& ip, uint16_t port, size_t thread_size)
    :_addr(ip, port)
     ,_accept_pool(this)
     ,_thread_pool(thread_size)
     ,_evenloop_m(thread_size, this)
{}

TcpServer::~TcpServer()
{
    this->stop();
}

void TcpServer::start()
{
    LL::LogInit();
    if(_socket.Bind(_addr) < 0)
    {
        FATAL_OUT << "socket bind error";
        ::exit(1);
    }

    if(_socket.Listen() < 0)
    {
        FATAL_OUT << "socket listen error";
        ::exit(2); 
    }

    _evenloop_m.putEventLoopToThreadPool(this->_thread_pool);
    auto ret = _accept_pool.addChannel(_socket.fd(), Channel::READ_EVENT);
    if(!ret)
    {
        FATAL_OUT << "socket add epoll error";
        ::exit(3);
    }
    ret->event_handle.setReadHandle(std::bind(&TcpServer::addConnect, this));

    _accept_pool.startLoop();
}

void TcpServer::stop()
{
    _evenloop_m.stopAllLoop();
    _accept_pool.stopLoop();
}

EventLoop* TcpServer::getNextLoop()
{
    return _evenloop_m.getNextLoop();
}

void TcpServer::addConnect()
{
    InetAddress peer;
    int fd = _socket.Accept(peer);

    auto pLoop = this->getNextLoop();
    if(!pLoop)
    {
        return;
    }

    auto pCh = pLoop->addChannel(fd, Channel::READ_EVENT);
    if(!pCh)
    {
        return;
    }
   
    auto id = getTcpConnectId();
    _conn_m.try_emplace(id, id, this, peer, pCh);
}

void TcpServer::removeConnect(uint64_t id)
{
    auto iter = _conn_m.find(id);
    if(_conn_m.end() == iter)
        return;

    iter->second.shutdownConnect();
    _conn_m.erase(iter);
}

uint64_t TcpServer::getTcpConnectId()
{
    static std::atomic_uint64_t ids(1);
    
    uint64_t ret = ids.fetch_add(1);

    return ret;
}
}
