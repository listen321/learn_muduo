#include "Channel.h"
#include "base/Logging.h"
#include "EventLoop.h"
#include "net/EventHandle.h"
#include <sys/epoll.h>
#include <unistd.h>

namespace LL
{

Channel::Channel(EventLoop* pLoop, const int fd)
              : _fd(fd)
              , _events(0)
              , _p_loop(pLoop)
{
}

Channel::~Channel()
{
    ::close(_fd);
}

void Channel::_enableEventChangeOp()
{
    if(_op == CHANNEL_OP::OP_NONE || _op == CHANNEL_OP::OP_DEL)
    {
        this->_setChannelOp(CHANNEL_OP::OP_ADD);
    }
    else 
    {
        this->_setChannelOp(CHANNEL_OP::OP_MOD);
    }
}

void Channel::_modEventChangeOp()
{
    if(this->isNoneEvent())
    {
        if (_op == CHANNEL_OP::OP_NONE) 
        {
            return;
        }
        else 
        {
            this->_setChannelOp(CHANNEL_OP::OP_DEL); 
        }
    }
    else 
    {
        this->_enableEventChangeOp();
    }
}

void Channel::enableReadEvents() 
{ 
    _events |=  READ_EVENT;  
    this->_enableEventChangeOp();
}

void Channel::disReadEvents()
{ 
    _events &= ~READ_EVENT;
    this->_modEventChangeOp();
}

void Channel::enableWirteEvents()
{ 
    _events |=  WRITE_EVENT;
    this->_enableEventChangeOp();
}

void Channel::disWiretEvents()   
{ 
    _events &= ~WRITE_EVENT;
    this->_modEventChangeOp();
}

void Channel::setEvent(EVENTS t)
{
    _events = t;
    this->_enableEventChangeOp();
}

void Channel::disAllEvents()
{ 
    _events = 0;
    this->_setChannelOp(CHANNEL_OP::OP_DEL); 
}

void Channel::eventAction()
{
    if((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
    {
        event_handle.eventCloseHandle();   
    }
    if(_revents & EPOLLERR)
    {
        event_handle.eventErrorHandle();
    }
    if(_revents & (EPOLLIN | EPOLLPRI | EPOLLHUP))
    {
        event_handle.eventReadHandle();
    }
    if(_revents & EPOLLOUT)
    {
        event_handle.eventWriteHandle();
    }

    this->setREvent(NONE_EVENT);
}

void Channel::removeChannel()
{
    this->_p_loop->removeChannel(this->_fd);
}

void Channel::updateEvent()
{
    this->_p_loop->updateEvent(*this);
}

}
