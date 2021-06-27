#include "LogFile.h"
#include <chrono>

namespace LL
{

LogFile::LogFile(const char* filename)
    :_appendfile(filename)
{
    auto now = std::chrono::system_clock::now();
    time_t s = std::chrono::system_clock::to_time_t(now);
    _last_hour = s / (60 * 60);
}

LogFile::~LogFile()
{}

void LogFile::writeFileUnlock(const char* buf, size_t len)
{
    if(this->isNeedReset())
    {
        this->setResetLogfile();
    }

    _appendfile.appendFileUnlocked(buf, len);
}

void LogFile::flushLogFile()
{
    _appendfile.fileFlush();
}

bool LogFile::isNeedReset()
{
    auto now = std::chrono::system_clock::now();

    time_t s = std::chrono::system_clock::to_time_t(now);
    time_t cur_h = s / (60 * 60);
    if(cur_h != _last_hour)
    {
        _last_hour = cur_h;
        return true;
    }
    size_t cur_w = _appendfile.getWritedBytes();
    size_t w_b = runconfig::instance().getCnfWriteBytes();
    if(cur_w >= w_b)
    {
        return true;
    }

    return false;
}

void LogFile::setResetLogfile()
{
    char logname[512] = {0};
    size_t r = runconfig::instance().getCurLogPathName(logname, sizeof(logname));
    if(r >= sizeof(logname))
        return;
    
    if(fs::exists(logname))
    {
        sprintf(logname+r, "-%zu", this->_tmp_no);
        ++_tmp_no;
    }
    else
    {
        _tmp_no = 1;
    }

    _appendfile.resetFile(logname);
}

}


