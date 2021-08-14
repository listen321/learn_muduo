#ifndef __LL_CHANNEL_H__
#define __LL_CHANNEL_H__

#include "base/Nocopyable.h"
#include "net/EventHandle.h"
#include <memory>
#include <sys/epoll.h>
#include <functional>

namespace LL
{
class EventLoop;
using EVENTS = decltype(std::declval<epoll_event>().events);
enum class CHANNEL_OP : uint32_t
{
    OP_NONE = 0,
    OP_ADD  = 1,
    OP_DEL  = 2,
    OP_MOD  = 3,
    MAX_OP,
};

enum class EVENT_TYPE : uint32_t
{
    EVENT_NONE   = 0,
    EVENT_SOCKET = 1,
    EVENT_ACCEPT = 2,
    EVENT_FD     = 3,
};


class Channel : public LL::NoCopyable
{
    public:
        Channel(EventLoop* pLoop, const int fd);

        ~Channel();

        const int getFd()  const { return _fd;     }
        EVENTS getEvents() const { return _events; }


        bool isReadEvents() const  { return _events & READ_EVENT;  }
        bool isWriteEvents() const { return _events & WRITE_EVENT; }
        bool isNoneEvent() const   { return _events & NONE_EVENT;  }


        void enableReadEvents();
        void disReadEvents();
        void enableWirteEvents();
        void disWiretEvents();
        void setEvent(EVENTS  t);
        void setREvent(EVENTS t) { _revents = t; }
        void disAllEvents();
        CHANNEL_OP getChannelOp() const { return _op; }
        EventLoop* getEventLoopPtr() { return _p_loop; }

        void eventAction();

        static constexpr uint32_t NONE_EVENT  = 0;
        static constexpr uint32_t READ_EVENT  = EPOLLIN | EPOLLPRI;
        static constexpr uint32_t WRITE_EVENT = EPOLLOUT;

        EventHandle                  event_handle;

        void removeChannel();
        void updateEvent();
    private:
        void _setChannelOp(const CHANNEL_OP op) { _op = op; }
        void _enableEventChangeOp();
        void _modEventChangeOp();
        const int                    _fd;
        EVENTS                       _events  = 0;
        EVENTS                       _revents = 0;
        EventLoop*                   _p_loop;
        CHANNEL_OP                   _op      = CHANNEL_OP::OP_NONE;
};

}
#endif
