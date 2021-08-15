#include "net/TcpConnect.h"
#include "net/TcpServer.h"
#include <iostream>

void recvCallback(LL::TcpConnect& conn, const std::string_view& data)
{
    std::cout << data << std::endl;
    // ::fflush(stdout);
    // conn.shutdownConnect();
    conn.sendMessage(data.data(), data.size());
}

int main()
{
    LL::TcpServer server(8888);
    server.setRecvCallback(recvCallback);
    server.start();

    return 0;
}
