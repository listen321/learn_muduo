#ifndef __LL_ENDIAN_H__
#define __LL_ENDIAN_H__

#include <bits/stdint-uintn.h>
#include <endian.h>


namespace LL 
{

namespace Sockets
{

inline uint64_t hostToNetwork64(uint64_t host)
{
    return htobe64(host);
}

inline uint32_t hostToNetwork32(uint32_t host)
{
    return htobe32(host);
}

inline uint16_t hostToNetwork16(uint16_t host)
{
    return htobe16(host);
}

inline uint64_t networkToHost64(uint64_t net)
{
    return be64toh(net);
}

inline uint32_t networkToHost32(uint32_t net)
{
    return be32toh(net);
}

inline uint16_t networkToHost16(uint16_t net)
{
    return be16toh(net);
}

}

}

#endif
