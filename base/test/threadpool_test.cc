#include "base/ThreadPool.h"
#include <iostream>
class Test
{
    public:
        int  a = 0;
        void func()
        {
            a += 1;
            std::cout << "a = "  << a <<  std::endl;
        }
};

int main()
{

    LL::ThreadPool p(3);
    Test t;
    p.enQueue(&Test::func, &t);
    p.enQueue(&Test::func, &t);
    p.enQueue(&Test::func, &t);

    return 0;
}
