#include "Epoll.h"
#include "base/Logging.h"
#include "net/Channel.h"
#include "base/Types.h"
#include <sys/epoll.h>
#include <unistd.h>

namespace LL
{
Epoll::Epoll(size_t event_size)
    : _epoll_fd(::epoll_create1(::EPOLL_CLOEXEC))
      ,_events(event_size)
{
    assert(_epoll_fd != -1);
}

Epoll::~Epoll()
{
    ::close(_epoll_fd);
}

void Epoll::epollWait(int timeout, std::vector<Channel*>& ret_vec)
{
    ret_vec.clear();
    int ret = ::epoll_wait(_epoll_fd, &(*_events.begin()), static_cast<int>(_events.size()),  timeout);

    if(ret < 0)
    {
        ERROR_OUT << "epoll_wait error, ret < 0 ";
        return;
    }
    if(ret == 0)
    {
        return;
    }

    for(int i = 0; i < ret; ++i)
    {
        Channel* ptr = static_cast<Channel*>(_events[0].data.ptr);
        ptr->setREvent(_events[0].events);
        ret_vec.push_back(ptr);
    }

    if (ret == _events.size())
    {
        _events.reserve(_events.size() * 2);
    }
}


int Epoll::updateEvent(const Channel& ch)
{
    auto op = ch.getChannelOp();
    epoll_event event;
    bzero(&event, sizeof(event));
    event.events   = ch.getEvents();
    event.data.ptr = const_cast<void*>(reinterpret_cast<const void*>(&ch));
    int fd = ch.getFd();

    int ret = -1;
    switch (op) {
        case CHANNEL_OP::OP_ADD:
            {
                ret = this->_addEvent(fd, &event);
            }
            break;
        case CHANNEL_OP::OP_DEL:
            {
                ret = this->_delEvent(fd, &event);
            }
            break;
        case CHANNEL_OP::OP_MOD:
            {
                ret = this->_modEvent(fd, &event);
            }
            break;
        default:
            break;
    }

    return ret;
}

int Epoll::_addEvent(int fd, epoll_event* event)
{
    return ::epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, event);
}

int Epoll::_delEvent(int fd, epoll_event* event)
{
    return ::epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, event);
}

int Epoll::_modEvent(int fd, epoll_event* event)
{
    return ::epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, event);
}

}
