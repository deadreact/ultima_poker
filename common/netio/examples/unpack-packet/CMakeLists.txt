cmake_minimum_required(VERSION 2.8)
project(unpack-packet)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

include_directories(
    ../../include
)

set(SOURCE_FILES
    ../../src/cryptotools.cpp
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
)
