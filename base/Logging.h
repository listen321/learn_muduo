#ifndef __LL_LOGGING_H__
#define __LL_LOGGING_H__
#include "base/LogStream.h"
#include "base/Nocopyable.h"
#include "base/Singleton.h"
#include <filesystem>
namespace LL
{

namespace detail
{
    extern size_t getCurLogName(char* buf, size_t size);
    extern void getCurTime(char* buf, size_t size, const char* fmt = "%F %T");
}

enum class DEBUG_LEVEL : uint8_t
{
    NONE  = 0,
    TRACE = 1,
    DEBUG = 2,
    INFO  = 3,
    WARN  = 4,
    ERROR = 5,
    FATAL = 6,
    MAX_LEVEL,
};

class SourceFile : LL::NoCopyable
{
    public:
        template<int  N>
        SourceFile(const char(&arr)[N])
            : _filename(arr),
              _size(N-1)
        {
            this->deleteExtraPath();
        }

        void deleteExtraPath()
        {
            const char* ret = ::strrchr(_filename, '/');
            if(!ret)
                return;

            _size -= static_cast<size_t>(ret - _filename);
            _filename = ret + 1;
        }

        inline const char* getFileName() const { return _filename; }
        inline size_t getSize() const { return _size; }

    private:
        const char* _filename;
        size_t      _size;
};

namespace fs = std::filesystem;
class RunConfig : public NoCopyable
{
    public:
        RunConfig();
        void setDebugLevel(DEBUG_LEVEL level);
        bool canLogOut(DEBUG_LEVEL level);

        constexpr static size_t BuffSize = 2;
        inline static fs::path LogPath = std::string(std::getenv("HOME")) + "/log";
        constexpr static size_t DefaultBytes = 20 * 1024 * 1024;
        constexpr static uint8_t Size = static_cast<uint8_t>(DEBUG_LEVEL::MAX_LEVEL);
        inline static const char* logLevelName[Size] = { "NONE",
                                                          "TRACE",
                                                          "DEBUG",
                                                          "INFO",
                                                          "WARN",
                                                          "ERROR",
                                                          "FATAL",
                                                        };
        const char* getLogLevelName(DEBUG_LEVEL level);

        size_t getCurLogPathName(char* buf, size_t size);
        void setCnfWriteBytes(size_t bytes);
        size_t getCnfWriteBytes();
    private:
        DEBUG_LEVEL _conf_level  = DEBUG_LEVEL::NONE;
        size_t      _write_bytes = DefaultBytes;
};

using runconfig = Singleton<RunConfig>;

#define TRACE_OUT if(LL::runconfig::instance().canLogOut(LL::DEBUG_LEVEL::TRACE)) \
    LL::Logging(__FILE__, __LINE__, LL::DEBUG_LEVEL::TRACE).logStream
#define DEBUG_OUT if(LL::runconfig::instance().canLogOut(LL::DEBUG_LEVEL::DEBUG)) \
    LL::Logging(__FILE__, __LINE__, LL::DEBUG_LEVEL::DEBUG).logStream
#define INFO_OUT if(LL::runconfig::instance().canLogOut(LL::DEBUG_LEVEL::INFO)) \
    LL::Logging(__FILE__, __LINE__, LL::DEBUG_LEVEL::DEBUG).logStream
#define WARN_OUT if(LL::runconfig::instance().canLogOut(LL::DEBUG_LEVEL::WARN)) \
    LL::Logging(__FILE__, __LINE__, LL::DEBUG_LEVEL::WARN).logStream
#define ERROR_OUT if(LL::runconfig::instance().canLogOut(LL::DEBUG_LEVEL::ERROR)) \
    LL::Logging(__FILE__, __LINE__, LL::DEBUG_LEVEL::ERROR).logStream

#define FATAL_OUT if(true) \
    LL::Logging(__FILE__, __LINE__, LL::DEBUG_LEVEL::FATAL).logStream

class Logging : public LL::NoCopyable
{
    public:
        Logging(const SourceFile& filename, size_t line, DEBUG_LEVEL level);
        ~Logging();

        LogStream  logStream;

        using OutLogFunc = void (*)(const char* , size_t);
        static void setOutLogFunc(OutLogFunc out);
        static void defaultOutLogFunc(const char* buf, size_t size);

        static OutLogFunc _out_func;
    private:
        void appendCurTime();
};
}
#endif
