#ifndef __LL_EVENT_HANDLE_H__
#define __LL_EVENT_HANDLE_H__
#include <functional>

namespace LL
{
    using EventCallback = std::function<void ()>;
}
#endif
