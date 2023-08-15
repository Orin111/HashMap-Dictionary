cmake_minimum_required(VERSION 3.21)
project(ex6_orin1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex6_orin1
        Dictionary.hpp
        HashMap.hpp
        tests_ex6_suchetzky.cpp)
