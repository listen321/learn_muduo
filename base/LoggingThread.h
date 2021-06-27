#ifndef __LL_LOGGING_THREAD_H__
#define __LL_LOGGING_THREAD_H__
#include "base/LogFile.h"
#include "CountDownLatch.h"
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>


namespace LL
{
    class LoggingThread
    {
        public:
            LoggingThread();
            ~LoggingThread();

            using UBPtr       = std::unique_ptr<CharBuffer<LARGE_BUFFER>>;
            using LogBuffVec  = std::vector<UBPtr>;

            LogBuffVec getFillLogBuffVec(size_t s);

            void start();
            void stop();
            void threadfunc();

            void logOutFunc(const char* data, size_t len);
        private:
            LogBuffVec               _log_buff_vec;
            LogBuffVec               _log_file_vec;
            LogBuffVec::iterator     _cur_iter;
            CountDownLatch           _latch;
            mutable std::mutex       _m;
            std::condition_variable  _cond;
            static constexpr size_t  _vec_size = 2;
            bool                     _stop = true;
    };

    class LogThread
    {
        public:
            LogThread():_log(), _t(&LoggingThread::threadfunc, &_log)
            {
            }
            ~LogThread()
            {
                _log.stop();
                if(_t.joinable())
                {
                    _t.join();
                }
            }

            void start()
            {
                _log.start();
            }

            LoggingThread& getLog()
            {
                return _log;
            }

        private:
            LoggingThread _log;
            std::thread   _t;
    };
    extern void LogInit();
}

#endif
