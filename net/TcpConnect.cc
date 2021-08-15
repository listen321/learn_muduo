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

    auto func = [thisid = id, pSer = pServer]()
    {
        pSer->removeConnect(thisid);
    };

    this->setRemoveCallback(func); 
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

    auto buff = _readbuf.getBuffer();
    if(buff.size() == 0)
    {
        this->closeSocket();
        return;
    }

    if(_recv_call)
    {
        _recv_call(*this, buff);
    }
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
    if(this->_remove_call)
    {
        //this->_p_server->removeConnect(this->_id);
        this->_remove_call();
    }
}

void TcpConnect::sendMessage(const char* buf, uint32_t len)
{
    if(len > BUFFER_SIZE)
    {
        return;
    }

    int ret = ::write(this->_p_ch->getFd(), buf, len);
    if(ret == len)
    {
        return;
    }

    //可能会覆盖上次数据
    if(ret < 0 && errno == EAGAIN)
    {
        this->_writebuf.setBuffer(buf, len);
    }
    else if(ret > 0)
    {
        this->_writebuf.setBuffer(buf + ret, len - ret);
    }
    else 
    {
        ERROR_OUT << "write error" ;
    }
}

}
