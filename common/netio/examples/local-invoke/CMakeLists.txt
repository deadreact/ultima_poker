cmake_minimum_required(VERSION 2.8)
project(local-invoke)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

include_directories(
    ../../include
)

set(SOURCE_FILES
    ../../src/asio_netio.cpp
    ../../src/netio.cpp
    ../../src/cryptotools.cpp
    ../../src/distributor.cpp
    ../../src/object_registry.cpp
    ../../src/service_handle.cpp
    main.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
    z
    sodium
    boost_system
)
