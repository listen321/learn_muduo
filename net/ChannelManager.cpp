#include "ChannelManager.h"
#include "net/Channel.h"

namespace LL
{

ChannelManager::ChannelManager()
{}

ChannelManager::~ChannelManager()
{}

Channel* ChannelManager::addChannel(EventLoop* pLoop, int fd, EVENTS event)
{
    auto iter = _channel_m.try_emplace(fd, pLoop, fd);
    if(!iter.second)
        return nullptr;

    iter.first->second.setEvent(event);
    _epoll_m.updateEvent(iter.first->second);
    return &(iter.first->second);
}

void ChannelManager::removeChannel(int fd)
{
    auto iter = _channel_m.find(fd);
    if(_channel_m.end() == iter)
        return;

    iter->second.disAllEvents();
    _epoll_m.updateEvent(iter->second);
    _channel_m.erase(fd);
}

}
