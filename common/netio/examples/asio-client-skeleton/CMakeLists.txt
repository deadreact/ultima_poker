cmake_minimum_required(VERSION 2.8)
project(asio-client-skeleton)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

include_directories(
    ../../include
)

#add_definitions(
#    -DBOOST_ASIO_ENABLE_HANDLER_TRACKING
#)

set(SOURCE_FILES
    ../../src/asio_netio.cpp
    ../../src/netio.cpp
    ../../src/cryptotools.cpp
    ../../src/distributor.cpp
    ../../src/object_registry.cpp
    ../../src/profdata.cpp
    ../../src/error.cpp
    ../../src/service_handle.cpp
    client.cpp
    main.cpp
)

set(HEADER_FILES
    ../../include/netio/netio.hpp
    ../../include/netio/invokers.hpp
    ../../include/netio/profdata.hpp
    ../../include/netio/error.hpp
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
