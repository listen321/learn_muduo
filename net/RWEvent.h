#ifndef __LL_RWEVENT_H__
#define __LL_RWEVENT_H__

namespace LL
{

template<typename T>
class RWEvent
{
    public:
        T& toChild()
        {
            return static_cast<T&>(*this);
        }

        void baseReadEvent(int fd)
        {
            this->toChild().readEvent(fd);
        }

        void baseWriteEvent(int fd)
        {
            this->toChild().writeEvent(fd);
        }
    private:
        RWEvent() {}
        friend T;
};

class RWSocket : public RWEvent<RWSocket>
{
    public:
        RWSocket() {}
        ~RWSocket() {}

        void readEvent(int fd)
        {

        }

        void writeEvent(int fd)
        {

        }
};

class RWTimer : public RWEvent<RWTimer>
{
    public:
        RWTimer() {}
        ~RWTimer() {}

        void readEvent(int fd)
        {

        }

        void writeEvent(int fd)
        {}
};
}

#endif
