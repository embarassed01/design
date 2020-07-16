cmake_minimum_required(VERSION 3.16)
project(cmn_cmake_mingw)

set(CMAKE_CXX_STANDARD 98)

add_executable(common_cmake_mingw main.cpp common-cmake-mingw.h common-cmake-minw.cpp)