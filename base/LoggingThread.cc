#include "LoggingThread.h"
namespace LL
{

using namespace std::chrono_literals;

LoggingThread::LoggingThread()
{
    _log_buff_vec = getFillLogBuffVec(_vec_size);
    _log_file_vec.reserve(_vec_size);
    _cur_iter = _log_buff_vec.begin();
}

LoggingThread::~LoggingThread()
{
    this->stop();
}

LoggingThread::LogBuffVec LoggingThread::getFillLogBuffVec(size_t s)
{
    LogBuffVec ret;
    ret.reserve(s);
    for(size_t i = 0; i < s; ++i)
    {
        UBPtr ptr = std::make_unique<CharBuffer<LARGE_BUFFER>>();
        if(!ptr)
            continue;
        ret.push_back(std::move(ptr));
    }

    return ret;
}

void LoggingThread::start()
{
    _stop = false;
    _latch.wait();
}

void LoggingThread::stop()
{
    _stop = true;
    _cond.notify_one();
}

void LoggingThread::threadfunc()
{
    char tmp[256];
    LL::runconfig::instance().getCurLogPathName(tmp, sizeof(tmp));
    LogFile logfile(tmp);
    LogBuffVec copy_vec = getFillLogBuffVec(_vec_size);
    _latch.countDown();
    assert(copy_vec.size());
    while(!_stop)
    {
        LogBuffVec tmp_vec;
        {
           std::unique_lock<std::mutex> lk(_m);
           _cond.wait_for(lk, 2s, [this] { return !_log_file_vec.empty(); });

           if(_log_file_vec.empty() && (*_cur_iter)->getCurSize() > 0)
           {
                _log_file_vec.push_back(std::move(*_cur_iter));
                ++_cur_iter;
           }

           tmp_vec.swap(_log_file_vec);

           //std::swap_ranges(_log_buff_vec.begin(), _cur_iter, copy_vec.begin());
           auto it_vec = _log_buff_vec.begin();
           for(auto c_it = copy_vec.begin(); it_vec != _cur_iter && c_it != copy_vec.end(); ++it_vec, ++c_it)
           {
               (*it_vec).swap(*c_it);
           }


           (*_cur_iter).swap(*_log_buff_vec.begin());
           if(it_vec != _cur_iter)
           {
               //删除临时分配空间
               _log_buff_vec.erase(it_vec, _log_buff_vec.end());
           }
           _cur_iter = _log_buff_vec.begin();
        }

        if(tmp_vec.size() > 10)
        {
            char tmpbuf[256] = {};
            char timebuf[64] = { 0 };
            LL::detail::getCurTime(timebuf, sizeof(timebuf));
            snprintf(tmpbuf, sizeof(tmpbuf), "%s:%s", timebuf, "too many log, this drop log");
            tmp_vec.resize(_vec_size);
        }

        size_t index = 0;
        for(auto& it : tmp_vec)
        {
            logfile.writeFileUnlock(it->data(), it->getCurSize());
            if(index < copy_vec.size())
            {
                it->clear();
                copy_vec[index++].swap(it);
            }
        }

        logfile.flushLogFile();
        tmp_vec.clear();
    }
}

void LoggingThread::logOutFunc(const char* data, size_t len)
{
    static_assert(LARGE_BUFFER > SMALL_BUFFER,
            "Buffer Size Error");

    std::unique_lock<std::mutex> lk(_m);
    if(_cur_iter == _log_buff_vec.end() || !(*_cur_iter))
    {
        goto END_ERROR;
    }

    if((*_cur_iter)->getAvail() > len)
    {
        (*_cur_iter)->append(data, len);
        return;
    }
   
    _log_file_vec.push_back(std::move(*_cur_iter));
    do
    {
        ++_cur_iter;
        //缓存使用完
        if(_cur_iter == _log_buff_vec.end())
        {
            UBPtr ptr = std::make_unique<CharBuffer<LARGE_BUFFER>>();
            if(!ptr)
                goto END_ERROR;
            _cur_iter = _log_buff_vec.insert(_cur_iter, std::move(ptr));
        }

        if(*_cur_iter)
        {
           (*_cur_iter)->append(data, len);
           break;
        }

    }while((*_cur_iter) != nullptr);

    _cond.notify_one();
    return;
    

    END_ERROR:
        ::fprintf(stderr, "%s", data);
}

static LogThread g_log_thread;
void logOut(const char* data, size_t len)
{
    g_log_thread.getLog().logOutFunc(data, len);
}

void LogInit()
{
    Logging::setOutLogFunc(logOut);
    g_log_thread.start();
}
}
