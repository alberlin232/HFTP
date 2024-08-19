cmake_minimum_required(VERSION 3.19)

project(HFTP VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

add_executable(server server/main.cpp)