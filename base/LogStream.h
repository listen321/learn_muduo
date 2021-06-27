
#ifndef __LL_LOGSTREAM_H__
#define __LL_LOGSTREAM_H__


#include "base/Types.h"
#include <algorithm>
#include <cstring>

namespace LL
{

#define SMALL_BUFFER 4000
#define LARGE_BUFFER 4000000

namespace detail
{

constexpr char  digits[] = "9876543210123456789";
constexpr char* zero = const_cast<char*>(digits) + 9;
static_assert(sizeof(digits) != 19, "error digits");

constexpr char  digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof(digitsHex) != 16, "error digitsHex");

template<typename T, typename U = std::enable_if_t<std::is_integral_v<T>>>
size_t convertIntegral(char buf[], T v)
{
    if(!buf)
        return 0;

    T i = v;
    char* p = buf;

    do{
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (v < 0)
    {
        *p++ = '-';
    }

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

template<typename T, typename U = std::enable_if_t<std::is_floating_point_v<T>>>
size_t convertFloat(char buf[], T v)
{
    if(!buf)
        return 0;

    int ret = snprintf(buf, 32, "%g", v);
    return ret;
}

template<typename T, typename U = std::enable_if_t<std::is_pointer_v<T>>>
size_t convertHex(char buf[], T v)
{
    if(!buf)
        return 0;

    uintptr_t i = reinterpret_cast<uintptr_t>(v);
    buf[0] = '0';
    buf[1] = 'x';
    char* s = &buf[2];
    char* p = s;

    do
    {
        int lsd = static_cast<int>(i % 16);
        i /= 16;
        *p++ = digitsHex[lsd];
    }while (i != 0);

    *p = '\0';
    std::reverse(s, p);

    return p - s + 2;
}

}

template<size_t SIZE>
class CharBuffer
{
    using Self = CharBuffer<SIZE>;
    public:
        CharBuffer()
            : _cur(_data)
        {
        }
        ~CharBuffer()
        {
        }

        CharBuffer(const CharBuffer<SIZE>& other)
        {
            size_t s = other.getCurSize();
            std::memcpy(this->_data, other._data, s);
            this->_cur = static_cast<char*>(_data) + s;
        }

        Self& operator=(const CharBuffer<SIZE>& other)
        {
            size_t s = other.getCurSize();
            std::memcpy(this->_data, other._data, s);

            this->_cur = static_cast<char*>(_data) + s;
        }

        size_t getCurSize() const
        {
            return static_cast<size_t>(_cur - static_cast<const char*>(_data));
        }

        size_t getAvail()
        {
            return SIZE - getCurSize();
        }

        void append(const char* data, size_t len)
        {
            if(!data)
                return;

            if(len >= getAvail())
                return;

            std::memcpy(_cur, data, len);
            _cur += len;
        }

        void reset()
        {
            _cur = _data;
        }

        void bZero()
        {
            bzero(_data, sizeof(_data));
        }

        void clear()
        {
            this->bZero();
            this->reset();
        }

        const char* data() const
        {
            return _data;
        }


        template<typename T>
        void appendIntegral(T v)
        {
            if(!canAppendStrNum())
                return;

            size_t s = detail::convertIntegral(_cur, v);
            _cur += s;
        }

        template<typename T>
        void appendFloat(T v)
        {
            if(!canAppendStrNum())
                return;

            size_t s = detail::convertFloat(_cur, v);
            _cur += s;
        }

        template<typename T>
        void appendPointer(T v)
        {
            if(!canAppendStrNum())
                return;

            size_t s = detail::convertHex(_cur, v);
            _cur +=  s;
        }

        bool canAppendStrNum()
        {
            return true;
        }
    private:
        char _data[SIZE] = {0};
        char* _cur;
};

class LogStream
{
    using SmallBuffer = CharBuffer<SMALL_BUFFER>;
    using Self = LogStream;
    public:
        template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        Self& operator<<(T v)
        {
            _buf.appendIntegral(v);
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>, int p0=0>
        Self& operator<<(T v)
        {
            _buf.appendPointer(v);
            return *this;
        }

        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>, int p0=0, int p1=0>
        Self& operator<<(T v)
        {
            _buf.appendFloat(v);
            return *this;
        }

        Self& operator<<(const char* data)
        {
            if(data)
            {
                _buf.append(data, strlen(data));
            }
            else
            {
                _buf.append("(null)", 6);
            }
            return *this;
        }

        Self& operator<<(char* data)
        {
            if(data)
            {
                _buf.append(data, strlen(data));
            }
            else
            {
                _buf.append("(null)", 6);
            }
            return *this;
        }

        Self& operator<<(char c)
        {
            _buf.append(&c, 1);
            return *this;
        }

        Self& operator<<(const string& str)
        {
            _buf.append(str.c_str(), str.size());
            return *this;
        }

        Self& operator<<(const bool f)
        {
            if(f == true)
            {
                _buf.append("true", 4);
            }
            else
            {
                _buf.append("false", 5);
            }
            return *this;
        }

        const char* getData() const
        {
            return _buf.data();
        }

        size_t getSize()
        {
            return _buf.getCurSize();
        }
    private:
        SmallBuffer _buf;
        constexpr static int  MaxShowNumber = 32;
};

}
#endif
