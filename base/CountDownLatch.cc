#include "CountDownLatch.h"
#include <mutex>

namespace LL
{
void CountDownLatch::countDown()
{
    std::unique_lock<std::mutex> m(this->_m);
    _count  -= 1;
    if (0 == _count)
    {
        m.unlock();
        _cond.notify_all();
    }
}

void CountDownLatch::wait()
{
    std::unique_lock<std::mutex> m(this->_m);
    _cond.wait(m, [&]{ return _count == 0; } );
}

unsigned int CountDownLatch::getCount() const
{
    std::unique_lock<std::mutex> m(this->_m);
    return _count;
}

}
