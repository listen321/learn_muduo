
#include "base/LogFile.h"
#include "base/Logging.h"
#include "base/LoggingThread.h"
#include <thread>

void testLog()
{
    LL::LogInit();
}
int main()
{
    using namespace std::chrono_literals;
    testLog();
    LL::CharBuffer<LARGE_BUFFER> buf;
    buf.append("testtesttest", 12);

    char tmp[256];
    LL::runconfig::instance().getCurLogPathName(tmp, sizeof(tmp));

    DEBUG_OUT << buf.data();
    DEBUG_OUT << tmp;
    DEBUG_OUT << "hahaha";

    int a = 10;
    std::string s{"this is an apple 啊哈哈"};
    INFO_OUT << "logtest adad" << a << s;
    //LL::LogFile flog(tmp);
    //flog.writeFileUnlock(buf.data(), buf.getCurSize());
    while(a < 100000)
    {
        DEBUG_OUT << "aaa, 123, 哈哈哈！！";
        ERROR_OUT <<  "a = " << a;
        ERROR_OUT <<  "&a = " << &a;
        a += 1; 

        std::this_thread::sleep_for(5ms);
    }

    return 0;
}
