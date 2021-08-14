#ifndef __LL_SOCKET_H__
#define __LL_SOCKET_H__

#include "base/Types.h"
#include "base/Logging.h"
#include "net/InetAddress.h"
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <fcntl.h>

namespace LL
{

class Socket
{
    public:
        explicit Socket(int fd)
            : _fd(fd)
        {}

        explicit Socket()
            : _fd(createNoBlockAndCloseOnExecSock())
        {

        }
        ~Socket()
        {
            ::close(_fd);
        }

        int fd() const { return _fd; }

        std::string getTcpInfo() const;

        int Listen() const;
        int Accept(InetAddress& addr) const;
        int Bind(const InetAddress& addr) const;

        static int createNoBlockAndCloseOnExecSock();
        static void setNoBlockAndCloseOnExecFd(int fd);
    private:
        const int _fd;
};

}

#endif
