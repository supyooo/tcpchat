cmake_minimum_required(VERSION 3.22.1)

project(tcp-server/client)

set(CMAKE_CXX_STANDARD 11) 

find_package(Boost REQUIRED COMPONENTS
system
log
log_setup)

add_executable(server server.cpp serverclient.cpp)
target_link_libraries(server ${Boost_LIBRARIES})

add_executable(client client.cpp serverclient.cpp)
target_link_libraries(client ${Boost_LIBRARIES})

add_executable(test Test.cpp serverclient.cpp)
target_link_libraries(test ${Boost_LIBRARIES} gtest)