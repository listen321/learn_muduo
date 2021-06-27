
#include "ThreadPool.h"
namespace LL
{

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

//void ThreadPool::start()
//{
//}

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

size_t ThreadPool::getQueueSize()
{
    std::unique_lock<std::mutex> m(this->_mutex);
    return _task_queue.size();
}

}
