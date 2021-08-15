project(net)

add_library(${PROJECT_NAME} STATIC
    Buffer.cc
    Channel.cc
    ChannelManager.cc
    Epoll.cc
    EventLoop.cc
    InetAddress.cc
    Socket.cc
    TcpConnect.cc
    TcpServer.cc
    TcpClient.cc
    )

add_library(light_net ALIAS ${PROJECT_NAME})
target_link_libraries(net base)

add_subdirectory(test)
