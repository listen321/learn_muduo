
add_executable(server_test server_test.cc)
target_link_libraries(server_test net)

add_executable(client_test client_test.cc)
target_link_libraries(client_test net)

