
#ifndef __LL_THREAD_POOL_H__
#define __LL_THREAD_POOL_H__

#include "base/Types.h"
#include "base/Nocopyable.h"
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <future>
#include <functional>
#include <memory>
#include <stdexcept>
namespace LL
{

class ThreadPool : public LL::NoCopyable
{
    public:
        ThreadPool(size_t s);
        ~ThreadPool();
        //void start();
        void threadPoolFunc();
        template<typename F, typename ...Args>
        auto enQueue(F&& f, Args&& ...args);

        size_t getQueueSize();
    private:
        std::vector<std::thread>  _thread_vec;
        std::mutex                _mutex;
        std::condition_variable   _cond;
        std::queue<std::packaged_task<void()>>  _task_queue;
        bool                      _stop = false;
};
}

#endif
