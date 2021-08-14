#ifndef __LL_CHANNEL_M_H__
#define __LL_CHANNEL_M_H__

#include "base/Nocopyable.h"
#include "net/Channel.h"
#include "net/Epoll.h"
#include <unordered_map>

namespace LL
{

class ChannelManager : public LL::NoCopyable
{
    public:
        ChannelManager();
        ~ChannelManager();

        using ChannelCon = std::unordered_map<int, Channel>;

        Channel* addChannel(EventLoop* pLoop, int fd, EVENTS event);

        void removeChannel(int fd);

        void epollWait(int timeout, std::vector<Channel*>& ret_vec)
        {
            _epoll_m.epollWait(timeout, ret_vec);
        }
        void updateEvent(Channel& ch)
        {
            _epoll_m.updateEvent(ch);
        }
    private:
        Epoll      _epoll_m;
        ChannelCon _channel_m;
};

}
#endif
