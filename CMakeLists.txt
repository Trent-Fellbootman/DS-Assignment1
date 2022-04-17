cmake_minimum_required(VERSION 3.10)
project(Assignment1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SOURCE_DIR)

add_subdirectory ("polynomial")

add_executable(main main.cpp)
target_link_libraries(main INTERFACE polynomial)