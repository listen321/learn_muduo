#include "net/TcpServer.h"

int main()
{
    LL::TcpServer server(8888);
    server.start();

    return 0;
}
