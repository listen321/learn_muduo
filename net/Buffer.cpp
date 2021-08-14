#include "Buffer.h"
#include <unistd.h>
#include <string.h>

namespace LL 
{

ReadBuffer::ReadBuffer()
{}

ReadBuffer::~ReadBuffer()
{}


bool ReadBuffer::readFd(int fd)
{
    _cursize = 0;

    int ret = ::read(fd, _buffer.data(), _buffer.size());
    if(ret < 0 || ret == _buffer.size())
    {
        return false;
    }
    _cursize = ret; 

    return true;
}

std::string_view ReadBuffer::getBuffer()
{
    return {_buffer.data(), _cursize};
}

// ---- ---- 
WriteBuffer::WriteBuffer()
{}

WriteBuffer::~WriteBuffer()
{}

bool WriteBuffer::setBuffer(const char* buf, size_t len)
{
    if(len > BUFFER_SIZE)
        return false;

    strncpy(_buffer.data(), buf, len);
    _cursize = len;
    _write_size = 0;

    return true;
}

bool WriteBuffer::writeFd(int fd)
{
    if(_write_size >= _cursize)
        return false;
    
    char* p_src = _buffer.data() + _write_size;
    int ret = ::write(fd, p_src, _cursize);
    if(ret < 0)
    {
        return false;
    }

    if(ret < _cursize)
    {
        _write_size = ret;
        return false;
    }

    _write_size = 0;
    _cursize    = 0;
    return true;
}

}
