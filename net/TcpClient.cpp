#include "TcpClient.h"
#include "net/Socket.h"

namespace LL
{

TcpClient::TcpClient(const string& ip, uint16_t port)
    :_server_addr(ip, port)
{}

TcpClient::~TcpClient()
{
    if(_fd != -1)
    {
        ::close(_fd);
    }
}

bool TcpClient::connectServer()
{
    _fd = Socket::createNoBlockAndCloseOnExecSock();

    int ret = ::connect(_fd, _server_addr.getSockAddr(), _server_addr.getSockAddrLen());
    if(ret != 0)
    {
        return false;
    }

    return true;
}

void TcpClient::start()
{
    if(_fd < 0)
        return;

    auto pCh = this->_event_loop.addChannel(_fd, Channel::READ_EVENT);
    _tcp_connect = std::make_unique<TcpConnect>(pCh);
    _event_loop.startLoop();
}



}
