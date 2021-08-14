#ifndef __LL_BUFFER_H__
#define __LL_BUFFER_H__

#include <array>
#include <bits/c++config.h>
#include <string_view>

namespace LL
{

#define BUFFER_SIZE 65535
class ReadBuffer
{
    public:
        ReadBuffer();
        ~ReadBuffer();

        bool readFd(int fd);
        std::string_view getBuffer();

    private:
        std::array<char, BUFFER_SIZE + 1>   _buffer {};
        std::size_t                         _cursize = 0;
};

class WriteBuffer
{
    public:
        WriteBuffer();
        ~WriteBuffer();

        bool setBuffer(const char* buf, size_t len);
        bool writeFd(int fd);

    private:
        std::array<char, BUFFER_SIZE + 1> _buffer {};
        std::size_t                       _cursize = 0;
        std::size_t                       _write_size = 0;
};
}

#endif
