cmake_minimum_required(VERSION 3.10)
project(Assignment1)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/polynomial)

add_executable(main main.cpp)
