#ifndef __LL_EVENT_HANDLE_H__
#define __LL_EVENT_HANDLE_H__


#include <atomic>
#include <functional>
#include <array>
#include <utility>
namespace LL
{

using EventCallback = std::function<void ()>;

enum class HANDLE_TYPE : size_t
{
    READ_TYPE  = 0,
    WRITE_TYPE = 1,
    ERROR_TYPE = 2,
    CLOSE_TYPE = 4,
    HANDLE_MAX,
};

class EventHandle
{
    public:
        void eventReadHandle()
        {
            this->_execCallback(HANDLE_TYPE::READ_TYPE);
        }
        void eventWriteHandle() 
        {
            this->_execCallback(HANDLE_TYPE::WRITE_TYPE);
        }
        void eventErrorHandle() 
        {
            this->_execCallback(HANDLE_TYPE::ERROR_TYPE);
        }
        void eventCloseHandle() 
        {
            this->_execCallback(HANDLE_TYPE::CLOSE_TYPE);
        }

        void setReadHandle(EventCallback call)
        {
            this->_setCallback(HANDLE_TYPE::READ_TYPE, call);
        }
        void setWriteHandle(EventCallback call) 
        {
            this->_setCallback(HANDLE_TYPE::WRITE_TYPE, call);
        }
        void setErrorHandle(EventCallback call) 
        {
            this->_setCallback(HANDLE_TYPE::ERROR_TYPE, call);
        }
        void setCloseHandle(EventCallback call) 
        {
            this->_setCallback(HANDLE_TYPE::CLOSE_TYPE, call);
        }


        using CallBackArr = std::array<EventCallback, static_cast<size_t>(HANDLE_TYPE::HANDLE_MAX)>;
    private:
        void _setCallback(HANDLE_TYPE t, EventCallback& call)
        {
            size_t type = static_cast<size_t>(t);
            _callback_arr[type] = std::move(call);
        }
        void _execCallback(HANDLE_TYPE t)
        {
            size_t type = static_cast<size_t>(t);
            if(_callback_arr[type])
            {
                _callback_arr[type]();
            }
        }
        CallBackArr _callback_arr;

};

}

#endif
