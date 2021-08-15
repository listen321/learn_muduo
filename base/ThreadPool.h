//Copyright (c) 2012 Jakob Progsch, Václav Zeman
//
//This software is provided 'as-is', without any express or implied
//warranty. In no event will the authors be held liable for any damages
//arising from the use of this software.
//
//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute it
//freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.

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


template<typename F, typename ...Args>
auto ThreadPool::enQueue(F&& f, Args&& ...args)
{
    using result_type = std::invoke_result_t<F, Args...>;
    std::packaged_task<result_type()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );
    std::future<result_type> ret = task.get_future();
    {
        std::unique_lock<std::mutex> m(this->_mutex);
        if(_stop)
        {
           throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        this->_task_queue.emplace(std::move(task));
    }
    this->_cond.notify_one();
    return ret;
}

inline size_t ThreadPool::getQueueSize()
{
    std::unique_lock<std::mutex> m(this->_mutex);
    return _task_queue.size();
}
}

#endif
