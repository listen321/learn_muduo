#ifndef __LL_NOCOPYABLE_H__
#define __LL_NOCOPYABLE_H__

namespace LL
{

class NoCopyable
{
    public:
       NoCopyable(const NoCopyable&) = delete;
       NoCopyable(const NoCopyable&&) = delete;

       NoCopyable& operator=(const NoCopyable&) = delete;
       NoCopyable& operator=(const NoCopyable&&) = delete;

    protected:
       NoCopyable() = default;
       ~NoCopyable() = default;
};

}
#endif
