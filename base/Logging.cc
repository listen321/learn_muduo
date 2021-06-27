#include "Logging.h"
#include <chrono>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <unistd.h>

namespace LL
{
namespace detail
{
    void getCurTime(char* buf, size_t size, const char* fmt)
    {
        const std::chrono::time_point<std::chrono::system_clock> now
            = std::chrono::system_clock::now();
        const std::time_t s_t = std::chrono::system_clock::to_time_t(now);
        struct tm tm_t;
        ::localtime_r(&s_t, &tm_t);
        std::strftime(buf, size, fmt, &tm_t);
    }

    size_t getCurProcName(char* buf, size_t size)
    {
        char tmp[512] = { 0 };
        size_t r = ::readlink("/proc/self/exe", tmp, sizeof(tmp));
        if(r == -1)
        {
            return 0;
        }

        const char* ret = ::strrchr(tmp, '/');
        if(!ret)
            return 0;

        int s = ::snprintf(buf, size, "%s", ret+1);
        if(s < 0)
            return 0;
        return s;
    }

    size_t getCurLogName(char* buf, size_t size)
    {
        size_t s = getCurProcName(buf, size);
        pid_t curpid = getpid();
        char* ret = buf + s;
        int p_s = ::sprintf(ret, ".%d.", curpid);
        if(p_s < 0)
            return p_s;

        char tmp[64] = { 0 };
        getCurTime(tmp, sizeof(tmp), "%Y%m%d-%H");
        int r_w =  ::snprintf(ret+p_s, sizeof(tmp), "%s.log", tmp);
        if(r_w < 0)
            return 0;
        return s + p_s + r_w;
    }

}

// class RunConfig
RunConfig::RunConfig()
{
    if(!fs::exists(LogPath))
    {
        fs::create_directories(LogPath);
    }

}

void RunConfig::setDebugLevel(DEBUG_LEVEL level)
{
    _conf_level = level;
}
bool RunConfig::canLogOut(DEBUG_LEVEL level)
{
    if(level < _conf_level)
        return false;

    return true;
}

const char* RunConfig::getLogLevelName(DEBUG_LEVEL level)
{
    uint8_t index = static_cast<uint8_t>(level);
    return this->logLevelName[index];
}

size_t RunConfig::getCurLogPathName(char* buf, size_t size)
{
    int r = ::sprintf(buf, "%s/", this->LogPath.c_str());
    if(r < 0)
        return 0;
    char* pos = buf + r;
    size_t s = size > r? size - r: 0;
    if(s == 0)
        return r;

    size_t r_l =  detail::getCurLogName(pos, size - r);
    return r + r_l; 
}

void RunConfig::setCnfWriteBytes(size_t bytes)
{
    this->_write_bytes = bytes;
}

size_t RunConfig::getCnfWriteBytes()
{
    return _write_bytes;
}

Logging::Logging(const SourceFile& filename, size_t line, DEBUG_LEVEL level)
{
    this->appendCurTime();
    logStream << filename.getFileName() << ":" << line << ":" << runconfig::instance().getLogLevelName(level) << ":";
}

Logging::~Logging()
{
    logStream<<"\n";
    _out_func(logStream.getData(), logStream.getSize());
}


void Logging::appendCurTime()
{
    char tmpbuf[64] = { 0 };
    detail::getCurTime(tmpbuf, sizeof(tmpbuf));
    logStream << tmpbuf << ":";
}

void Logging::setOutLogFunc(OutLogFunc out)
{
    _out_func = out;
}

void Logging::defaultOutLogFunc(const char* buf, size_t size)
{
    ::fwrite(buf, 1, size, stdout);
}

Logging::OutLogFunc Logging::_out_func = Logging::defaultOutLogFunc;

}
