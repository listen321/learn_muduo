#ifndef __LL_COUNT_DOWN_LATCH_H__
#define __LL_COUNT_DOWN_LATCH_H__

#include <mutex>
#include <condition_variable>

namespace LL
{

class CountDownLatch
{
    public:
        explicit CountDownLatch(unsigned int n = 1) :
            _count(n)
        {}

        void countDown();
        void wait();

        unsigned int getCount() const;
    private:
        unsigned int                _count;
        mutable std::mutex          _m;
        std::condition_variable     _cond;
};

}
#endif
