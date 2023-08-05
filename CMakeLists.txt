cmake_minimum_required(VERSION 3.10)
project(DDoS)

set(CMAKE_CXX_STANDARD 14)

add_executable(DDoS
        Ddoser.cpp
       )

target_link_libraries(DDoS pthread)