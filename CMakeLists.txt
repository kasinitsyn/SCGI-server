cmake_minimum_required(VERSION 3.5)

project(server_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp async_server.hpp parser.hpp parser.cpp session.hpp session.cpp)  #async_server.cpp

#add_library(libevent STATIC IMPORTED)
#set_property(TARGET libevent PROPERTY IMPORTED_LOCATION "/usr/lib/x86_64-linux-gnu/libevent.a")

#target_link_libraries(${PROJECT_NAME} PUBLIC libevent)

find_package(Boost 1.68.0 REQUIRED COMPONENTS coroutine thread system)
target_link_libraries(${PROJECT_NAME} Boost::coroutine Boost::thread Boost::system)
