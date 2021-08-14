#ifndef __LL_TCP_CONNECT_H__
#define __LL_TCP_CONNECT_H__

#include "net/InetAddress.h"
#include "net/Buffer.h"
#include <memory>


namespace LL 
{

//确保Channel 生命周期长于TcpConnect
class Channel;
class TcpServer;
class TcpConnect 
{
    public:
        TcpConnect(uint64_t id, TcpServer* pServer, InetAddress& peer_addr, Channel* p_ch);
        TcpConnect(Channel* p_ch);
        ~TcpConnect();

        TcpConnect(const TcpConnect& other) = default;
        TcpConnect& operator=(const TcpConnect& other) = default;
        TcpConnect(TcpConnect&& other) = default;
        TcpConnect& operator=(TcpConnect&& other) = default;

        void readSocket();
        void writeSocket();
        void closeSocket();
        void errorHandle();

        void shutdownConnect();
        void removeConnect();
    private:
        uint64_t               _id = 0;
        TcpServer*             _p_server = nullptr;
        InetAddress            _peer_addr;
        Channel*               _p_ch;
        ReadBuffer             _readbuf;
        WriteBuffer            _writebuf;
};


}

#endif
