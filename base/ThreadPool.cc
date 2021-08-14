
#include "ThreadPool.h"
namespace LL
{

ThreadPool::ThreadPool(size_t s)
    :_stop(false)
{
    for(auto i = 0; i < s; ++i)
    {
        _thread_vec.emplace_back(&ThreadPool::threadPoolFunc, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> m(_mutex);
        this->_stop = true;
    }
    this->_cond.notify_all();
    for(auto& it : _thread_vec)
    {
        it.join();
    }
}

void ThreadPool::threadPoolFunc()
{
    for(;;)
    {
        std::packaged_task<void()> task;
        {
            std::unique_lock<std::mutex> m(_mutex);
            _cond.wait(m ,
                    [this] {return _stop || !_task_queue.empty();});

            if(_stop && _task_queue.empty())
            {
                return;
            }

            task = std::move(this->_task_queue.front());
            this->_task_queue.pop();
        }
        task();
    }
}

}
