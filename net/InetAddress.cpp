#include "InetAddress.h"
#include "net/Endian.h"
#include <netinet/in.h>
#include <strings.h>

namespace LL
{

InetAddress::InetAddress()
{
    bzero(&_addr, sizeof(_addr));
}

InetAddress::InetAddress(uint16_t port, in_addr_t in_addr)
{
    bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port   = Sockets::hostToNetwork16(port);
    _addr.sin_addr.s_addr = Sockets::hostToNetwork32(in_addr);
}

InetAddress::InetAddress(const std::string& ip, uint16_t port) : InetAddress(port, 0)
{
    ::inet_pton(AF_INET, ip.data(), &_addr.sin_addr);
}

InetAddress::~InetAddress()
{
}

uint16_t InetAddress::getPort()
{
    return Sockets::networkToHost16(_addr.sin_port);
}

string InetAddress::getIp()
{
    char buf[INET_ADDRSTRLEN] = { 0 };
    ::inet_ntop(AF_INET, &_addr.sin_addr, buf, INET_ADDRSTRLEN);

    return { buf };
}

}
