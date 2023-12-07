cmake_minimum_required(VERSION 2.8)
project(asio-timer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

include_directories(
    ../../include
)

set(SOURCE_FILES
    ../../src/asio_netio.cpp
    main.cpp
)

set(HEADER_FILES
    ../../include/netio/timer.hpp
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
    z
    sodium
    boost_system
)
