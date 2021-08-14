#ifndef __LL_EVENT_LOOP_H__
#define __LL_EVENT_LOOP_H__

#include "ChannelManager.h"
#include "EventHandle.h"
#include "Epoll.h"
#include "net/Channel.h"
#include <bits/stdint-uintn.h>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include <atomic>

namespace LL
{
class TcpServer;
class EventLoop : LL::NoCopyable
{
    public:
        EventLoop(TcpServer* p_server);
        EventLoop();
        ~EventLoop();

        void startLoop();
        void stopLoop();
        void evenLoop();

        Channel* addChannel(int fd, EVENTS event);
        uint32_t getEventLoopId() const { return _self_id; }

        bool isSameEventLoop(const EventLoop& other);

        void addEventFunc(EventCallback func);
        void weakup();
        void execFunc();

        void eventfdRead();
        static uint32_t getCurEventLoopId();
        static int getEventFd();

        void removeChannel(int fd) 
        {   
            this->_all_channel.removeChannel(fd);
        }

        void updateEvent(Channel& ch)
        {
            this->_all_channel.updateEvent(ch);
        }

    private:
        mutable std::mutex           _m;
        bool                         _stop = true;
        std::vector<EventCallback>   _func_vec;
        std::vector<EventCallback>   _func_vec_tmp;
        ChannelManager               _all_channel;
        TcpServer*                   _p_server = nullptr; //获取全局的信息
        uint32_t                     _self_id = 0;
        std::vector<Channel*>        _epoll_ret;
        int                          _event_fd = -1;

        static constexpr int TimeOutMS = 10000;

};

}

#endif
