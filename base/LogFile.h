#ifndef __LL_LOGFILE_H__
#define __LL_LOGFILE_H__

#include "base/FileUtil.h"
#include "base/LogStream.h"
#include "base/Nocopyable.h"
#include "base/Logging.h"

namespace LL
{

class LogFile : LL::NoCopyable
{
    public:
        LogFile(const char* filename);
        ~LogFile();

        void writeFileUnlock(const char* buf, size_t len);
        void flushLogFile();

        bool isNeedReset();
        void setResetLogfile();

    private:
        AppendFile               _appendfile;
        inline static size_t     _tmp_no      = 1;
        time_t                   _last_hour   = 0;
};

}

#endif
