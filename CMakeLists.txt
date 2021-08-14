cmake_minimum_required(VERSION 3.20)


set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(${PROJECT_NAME}
                         -g
                         -Wall
                        )

project(LOGTEST LANGUAGES CXX)

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(base)
add_subdirectory(net)
