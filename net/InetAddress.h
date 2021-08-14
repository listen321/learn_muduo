
#ifndef __LL_INET_ADDRESS_H__
#define __LL_INET_ADDRESS_H__

#include "base/Types.h"
#include "net/Endian.h"
#include <netinet/in.h>
#include <arpa/inet.h>

namespace LL
{

class InetAddress
{
    public:
        InetAddress();
        explicit InetAddress(uint16_t port, in_addr_t  in_addr);
        explicit InetAddress(const std::string& ip, uint16_t port);
        ~InetAddress();

        uint16_t getPort();
        string getIp();

        sockaddr* getSockAddr() const { return const_cast<sockaddr*>(reinterpret_cast<const sockaddr*>(&_addr)); }
        socklen_t getSockAddrLen() const { return sizeof(_addr); };

    private:
       struct sockaddr_in _addr;
};

}
#endif

