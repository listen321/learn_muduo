#include "TcpConnect.h"
#include "net/Channel.h"
#include "net/TcpServer.h"
#include "base/Logging.h"

namespace  LL
{

TcpConnect::TcpConnect(uint64_t id, TcpServer* pServer, InetAddress& peer_addr, Channel* p_ch)
    : _id(id), _p_server(pServer), _peer_addr(peer_addr),
      _p_ch(p_ch)
{
    _p_ch->event_handle.setReadHandle(std::bind(&TcpConnect::readSocket, this));
    _p_ch->event_handle.setWriteHandle(std::bind(&TcpConnect::writeSocket, this)); 
    _p_ch->event_handle.setErrorHandle(std::bind(&TcpConnect::errorHandle, this));
    _p_ch->event_handle.setCloseHandle(std::bind(&TcpConnect::closeSocket, this));
    DEBUG_OUT << "new connect";
}

TcpConnect::TcpConnect(Channel* p_ch)
    : _p_ch(p_ch)
{
    _p_ch->event_handle.setReadHandle(std::bind(&TcpConnect::readSocket, this));
    _p_ch->event_handle.setWriteHandle(std::bind(&TcpConnect::writeSocket, this)); 
    _p_ch->event_handle.setErrorHandle(std::bind(&TcpConnect::errorHandle, this));
    _p_ch->event_handle.setCloseHandle(std::bind(&TcpConnect::closeSocket, this));
    DEBUG_OUT << "new connect";

}

TcpConnect::~TcpConnect()
{

}

void TcpConnect::readSocket()
{
    bool is_read = _readbuf.readFd(_p_ch->getFd());
    if(!is_read)
    {
        ERROR_OUT << "socket read error";
        return;
    }

    std::string s(_readbuf.getBuffer());
    DEBUG_OUT << is_read << "msg: " << s;
}

void TcpConnect::writeSocket()
{
    bool is_write = _writebuf.writeFd(_p_ch->getFd());
    if(is_write)
    {
        _p_ch->disWiretEvents();
        _p_ch->updateEvent();
    }
}

void TcpConnect::closeSocket()
{
    this->removeConnect();
}

void TcpConnect::errorHandle()
{
    this->removeConnect();
}

void TcpConnect::shutdownConnect()
{
    this->_p_ch->removeChannel();
}

void TcpConnect::removeConnect()
{
    this->_p_server->removeConnect(this->_id);
}

}
