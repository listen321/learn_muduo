#ifndef __LL_EPOLL_H__
#define __LL_EPOLL_H__

#include <vector>
#include <sys/epoll.h>
#include <optional>
#include "Channel.h"

namespace LL
{

class Epoll
{
    public:
        Epoll(size_t event_size = 16);
        ~Epoll();

        void epollWait(int timeout, std::vector<Channel*>& ret_vec);
        int updateEvent(const Channel& ch);

    private:
        int _addEvent(int fd, epoll_event* event);
        int _delEvent(int fd, epoll_event* event);
        int _modEvent(int fd, epoll_event* event);

        int _epoll_fd = -1;
        std::vector<epoll_event> _events;
};

}
#endif
