#include "EventLoop.h"
#include "base/Logging.h"
#include <mutex>
#include <sys/eventfd.h>

namespace LL
{
EventLoop::EventLoop(TcpServer* p_server)
    : _p_server(p_server)
      , _self_id(getCurEventLoopId())
      , _event_fd(getEventFd())
{
    auto ret = this->addChannel(_event_fd, Channel::READ_EVENT);
    if(ret)
    {
        ret->event_handle.setReadHandle(std::bind(&EventLoop::eventfdRead, this));
    }
}

EventLoop::EventLoop()
    : _event_fd(getEventFd())
{
    auto ret = this->addChannel(_event_fd, Channel::READ_EVENT);
    if(ret)
    {
        ret->event_handle.setReadHandle(std::bind(&EventLoop::eventfdRead, this));
    }
}

EventLoop::~EventLoop()
{
}

void EventLoop::startLoop()
{
    _stop = false;
    this->evenLoop();
}

void EventLoop::evenLoop()
{
    while(!_stop)
    {
        _all_channel.epollWait(TimeOutMS, _epoll_ret);
        for(auto it : _epoll_ret)
        {
            it->eventAction();
        }

        this->execFunc();
    }
}

void EventLoop::stopLoop()
{
    _stop = true;
}

Channel* EventLoop::addChannel(int fd, EVENTS event)
{
    auto ret = this->_all_channel.addChannel(this,
            fd, event);
    if (!ret)
    {
        ERROR_OUT << "add Channel failed !!! fd: " << fd;
    }

    return ret;
}

bool EventLoop::isSameEventLoop(const EventLoop& other)
{
    return this->_self_id == other.getEventLoopId();
}

void EventLoop::addEventFunc(EventCallback func)
{
    std::unique_lock<std::mutex> _m;
    _func_vec.push_back(std::move(func));
}

void EventLoop::execFunc()
{
    {
        std::unique_lock<std::mutex> lc;
        _func_vec_tmp.clear();
        _func_vec_tmp.swap(_func_vec);
    }

    for(auto& it : _func_vec_tmp)
    {
        it();
    }
}

void EventLoop::weakup()
{
    int ret = ::eventfd_write(_event_fd, 1);
    if(ret < 0)
    {
        ERROR_OUT << "eventfd_write failed";
    }
}

void EventLoop::eventfdRead()
{
    eventfd_t tmp = 0;
    int ret = ::eventfd_read(_event_fd, &tmp);
    if(ret < 0)
    {
        ERROR_OUT << "eventfd_read failed";
    }
}

uint32_t EventLoop::getCurEventLoopId()
{
    static std::atomic_uint32_t cur_id(1);

    return cur_id.fetch_add(1);
}

int EventLoop::getEventFd()
{
    int ret = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if(ret < 0)
    {
        ERROR_OUT << "create eventfd failed";
        abort();
    }

    return ret;
}


}

