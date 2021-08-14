#ifndef __LL_EVENT_LOOP_M_H__
#define __LL_EVENT_LOOP_M_H__

#include "base/ThreadPool.h"
#include "net/EventLoop.h"
#include "base/Singleton.h"
#include <cstdint>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>

namespace LL
{

class ThreadPool;
class EventLoopM : public LL::NoCopyable
{
    public:
        EventLoopM(size_t s, TcpServer* p_server)
            : _index(0)
        {
            for(auto i = 0; i < s; ++i)
            {
                _evenloop_vec.emplace_back(std::make_unique<EventLoop>(p_server));
            }
        }
        ~EventLoopM()
        {
        }

        EventLoop* getNextLoop()
        {
            if(_evenloop_vec.empty())
            {
                return nullptr;
            }

            uint32_t cur_index = _index;
            {
                std::unique_lock<std::mutex> l(_m_index);
                _index += 1;
                if(_index >= _evenloop_vec.size())
                {
                    _index = 0;
                }
            }

            return _evenloop_vec[cur_index].get();
        }

        void putEventLoopToThreadPool(ThreadPool& pool)
        {
            for(auto& it : _evenloop_vec)
            {
                pool.enQueue(&EventLoop::startLoop, it.get()); 
            }
        }
    
        void stopAllLoop()
        {
            for(auto& it : _evenloop_vec)
            {
                it->stopLoop();
            }
        }
    private:
        std::uint32_t          _index = 0;
        std::mutex             _m_index;
        std::vector<std::unique_ptr<EventLoop>> _evenloop_vec;
};

}
#endif
