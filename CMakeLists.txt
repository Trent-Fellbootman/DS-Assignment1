cmake_minimum_required(VERSION 3.10)
project(Assignment1)

add_subdirectory(polynomial)

add_executable(main main.cpp)
target_link_libraries(main polynomial)
