#include "FileUtil.h"

namespace LL 
{

AppendFile::AppendFile(const string& filename)
    : _fp(::fopen(filename.c_str(), "ae")), //"e" O_CLOEXEC
       _write_bytes(0) 
{
    if(!_fp)
    {
        std::perror(filename.c_str());
        return;
    }


    ::setbuffer(_fp, _buf, sizeof(_buf));
}

AppendFile::AppendFile(const char* filename)
    : _fp(::fopen(filename, "ae")), //"e" O_CLOEXEC
       _write_bytes(0) 
{
    if(!_fp)
    {
        std::perror(filename);
        return;
    }

    ::setbuffer(_fp, _buf, sizeof(_buf));
}

AppendFile::~AppendFile()
{
    this->cleanFp();
}

void AppendFile::appendFile(const char* buf, const size_t len)
{
    if(!_fp)
        return;
    size_t r = ::fwrite(buf, 1, len, _fp);
    if(r != len)
    {
        std::perror("not write finished");
    }
    _write_bytes += len;
}

void AppendFile::appendFileUnlocked(const char* buf, const size_t len)
{
    if(!_fp)
        return;
    size_t r = ::fwrite_unlocked(buf, 1, len, _fp);
    if(r != len)
    {
        std::perror("not write finished");
    }
    _write_bytes += len;
}

void AppendFile::fileFlush()
{
    if(!_fp)
        return;
    ::fflush(_fp);
}

void AppendFile::cleanFp()
{
    if(!_fp)
    {
        return;
    }

    ::fclose(_fp); 
    _fp = nullptr;
    _write_bytes = 0;
    ::bzero(_buf, sizeof(_buf));
}

void AppendFile::resetFile(const string& filename)
{
    this->resetFile(filename.c_str());
}

void AppendFile::resetFile(const char* filename)
{
    this->cleanFp();
    _fp = ::fopen(filename, "ae");
    if(!_fp)
    {
        std::string s{"fopen fail "};
        s += filename;
        std::perror(s.c_str());
        return;
    }

    ::setbuffer(_fp, _buf, sizeof(_buf));
}

}
