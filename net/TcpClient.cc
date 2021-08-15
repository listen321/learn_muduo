#include "TcpClient.h"
#include "net/Socket.h"

namespace LL
{

TcpClient::TcpClient(const string& ip, uint16_t port)
    :_server_addr(ip, port)
{}

TcpClient::~TcpClient()
{
    this->closeSocket();
}

bool TcpClient::connectServer()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (_fd < 0)
    {
        std::perror("create socket failed");
        return false;
    }

    int ret = ::connect(_fd, _server_addr.getSockAddr(), _server_addr.getSockAddrLen());
    if(ret != 0)
    {
        std::perror("tcp connect failed!!");
        return false;
    }
    Socket::setNoBlockAndCloseOnExecFd(_fd);

    auto pCh = this->_event_loop.addChannel(_fd, Channel::READ_EVENT);
    if(!pCh)
    {
        return false;
    }
    _tcp_connect = std::make_unique<TcpConnect>(pCh);
    _tcp_connect->setRemoveCallback(std::bind(&TcpClient::peerClose, this));
    if(_call)
    {
        _tcp_connect->setRecvCallback(_call);
    }

    return true;
}

void TcpClient::start()
{
    if(_fd < 0)
        return;

    _event_loop.startLoop();
}

void TcpClient::sendMsg(const std::string& s)
{
    if(!_tcp_connect)
        return;
    
    _tcp_connect->sendMessage(s.data(), s.size());
}

void TcpClient::peerClose()
{
    this->_closeSocket();
    DEBUG_OUT << "peer close socket";
    std::exit(0);
}

void TcpClient::shutdownConnect()
{
    this->_closeSocket();
}

void TcpClient::closeSocket()
{
    if(_fd != -1)
    {
        ::close(_fd);
        _fd = -1;
    }
}

void TcpClient::_closeSocket()
{
    this->_tcp_connect->shutdownConnect();
    _tcp_connect.reset();
    _fd = -1;
}
}
