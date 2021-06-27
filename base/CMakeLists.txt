project(base)

add_library(${PROJECT_NAME} STATIC
    CountDownLatch.cc
    FileUtil.cc
    Logging.cc
    LogFile.cc
    ThreadPool.cc
    LoggingThread.cc
    )

#target_include_directories(${PROJECT_NAME} PRIVATE
#    ${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} pthread)
add_library(light_base ALIAS ${PROJECT_NAME})

add_subdirectory(test)
