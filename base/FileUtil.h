
#ifndef __LL_FILEUNIT_H__
#define __LL_FILEUNIT_H__

#include "base/Nocopyable.h"
#include "base/Types.h"
namespace LL
{

class AppendFile : LL::NoCopyable
{
    public:
        AppendFile(const string& filename);
        AppendFile(const char* filename);
        ~AppendFile();

        void appendFile(const char* buf, const size_t len);
        void appendFileUnlocked(const char* buf, const size_t len);
        void fileFlush();
        size_t getWritedBytes() { return _write_bytes; }
        void cleanFp();
        void resetFile(const string& filename);
        void resetFile(const char* filename);
    private:
        FILE*   _fp = nullptr;
        char    _buf[4*1024] = {0};
        size_t  _write_bytes = 0;
};

}

#endif
