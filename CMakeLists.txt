cmake_minimum_required(VERSION 3.10)
project(Assignment1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SOURCE_DIR)

if (CMAKE_CXX_PLATFORM_ID STREQUAL Darwin)
  include_directories("/usr/local/include")
endif()

add_subdirectory ("polynomial")
add_subdirectory ("test")
add_subdirectory("application")

add_executable(main main.cpp)
target_compile_options(main PRIVATE -g)
target_link_libraries(main INTERFACE polynomial PUBLIC application)

