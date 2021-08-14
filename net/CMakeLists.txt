project(net)

add_library(${PROJECT_NAME} STATIC
    Buffer.cpp
    Channel.cpp
    ChannelManager.cpp
    Epoll.cpp
    EventLoop.cpp
    InetAddress.cpp
    Socket.cpp
    TcpConnect.cpp
    TcpServer.cpp
    )

add_library(light_net ALIAS ${PROJECT_NAME})
target_link_libraries(net base)

add_subdirectory(test)
