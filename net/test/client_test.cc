
#include "net/TcpClient.h"
#include <iostream>

void recvCallback(LL::TcpConnect& conn, const std::string_view& data)
{
    std::cout << data << std::endl;
    std::string s;
    std::cin >> s;
    conn.sendMessage(s.c_str(), s.size());
}

int main()
{
    LL::TcpClient client("127.0.0.1", 8888);
    client.setRecvCallback(recvCallback);
    if(!client.connectServer())
    {
        return 0;
    }
    client.sendMsg("connect"); 

    client.start();  
    
    return 0;
}
