#include "../Logging.h"
#include "../Logging.cc"

#include <iostream>
using namespace std;
int main()
{
    int x = 10;
    DEBUG_OUT << "hehe" << " " << "haha"<< " ]sdasd" << x << " ";
    std::string s {"how are you"};

    DEBUG_OUT << s;

    bool f = false;
    WARN_OUT << f;

    int a = 10;
    FATAL_OUT << &a; 

    char buf[1024] = {0};
    LL::detail::getCurLogName(buf, sizeof(buf));

    FATAL_OUT << buf; 
    //LL::SourceFile s("test/logname");
    //LL::Logging test_log(s, 10, LL::DEBUG_LEVEL::DEBUG);

    //std::cout << test_log.logStream.getData() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(30));

    return 0;
}
