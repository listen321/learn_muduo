#ifndef __LL_SINGLETON_H__
#define __LL_SINGLETON_H__
#include "base/Nocopyable.h"
#include <thread>
namespace LL {
template<typename T>
class Singleton : LL::NoCopyable
{
    public:
        Singleton() = delete;
        ~Singleton() = delete;

        static T& instance()
        {
            return _val;
        }

    private:
        inline static T _val;
};
}
#endif
