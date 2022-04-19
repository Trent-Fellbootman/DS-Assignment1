cmake_minimum_required(VERSION 3.10)
project(polynomial-calculator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SOURCE_DIR)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

if (CMAKE_CXX_PLATFORM_ID STREQUAL Darwin)
  include_directories("/usr/local/include")
endif()

add_subdirectory ("polynomial")
add_subdirectory ("test")
add_subdirectory("application")

add_executable(polycal main.cpp)
target_compile_options(polycal PRIVATE -g)
target_link_libraries(polycal INTERFACE polynomial PUBLIC application)

