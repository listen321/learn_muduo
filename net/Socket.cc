
#include "Socket.h"
#include <fcntl.h>

namespace LL
{

std::string Socket::getTcpInfo() const
{
    struct tcp_info t;
    bzero(&t, sizeof(t));
    socklen_t l = sizeof(t);
    if(::getsockopt(_fd, SOL_TCP, TCP_INFO, &t, &l) < 0)
    {
        return "";
	}

    char buf[512] = {0};
 	snprintf(buf, l, "unrecovered=%u "                                            
    			"rto=%u ato=%u snd_mss=%u rcv_mss=%u "
    			"lost=%u retrans=%u rtt=%u rttvar=%u "
    			"sshthresh=%u cwnd=%u total_retrans=%u",
    			t.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
    			t.tcpi_rto,          // Retransmit timeout in usec
    			t.tcpi_ato,          // Predicted tick of soft clock in usec
    			t.tcpi_snd_mss,
    			t.tcpi_rcv_mss,
    			t.tcpi_lost,         // Lost packets
    			t.tcpi_retrans,      // Retransmitted packets out
    			t.tcpi_rtt,          // Smoothed round trip time in usec
    			t.tcpi_rttvar,       // Medium deviation
    			t.tcpi_snd_ssthresh,
    			t.tcpi_snd_cwnd,
    			t.tcpi_total_retrans);  // Total retransmits for entire connection    

    return buf;
}

int Socket::Listen() const
{
	return ::listen(_fd, SOMAXCONN);
}

int Socket::Accept(InetAddress& addr) const
{
	socklen_t len 	= addr.getSockAddrLen();
	sockaddr* pAddr = addr.getSockAddr();
	return ::accept(_fd, pAddr, &len);
}

int Socket::Bind(const InetAddress& addr) const
{
	socklen_t len 	= addr.getSockAddrLen();
	sockaddr* pAddr = addr.getSockAddr();
    return ::bind(_fd, pAddr, len);
}

int Socket::createNoBlockAndCloseOnExecSock()
{
    int ret = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (ret < 0)
    {
        FATAL_OUT << "create socket error"; 
        ::abort();
        return ret;
    }

    setNoBlockAndCloseOnExecFd(ret);
    return ret;
}

void Socket::setNoBlockAndCloseOnExecFd(int fd)
{
    int f = ::fcntl(fd, F_GETFL);
    if(f < 0)
    {
        FATAL_OUT << "setNoBlockAndCloseOnExecFd F_GETFL error"; 
        ::abort();
    }
    
    f |= O_NONBLOCK | FD_CLOEXEC;
    if(::fcntl(fd, F_SETFL, f) < 0)
    {
        FATAL_OUT << "setNoBlockAndCloseOnExecFd F_SETFL error"; 
        ::abort();
    }
}

}
