cmake_minimum_required(VERSION 3.14)
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
set(test_name test_polynomial)
add_executable(${test_name} test.cpp)
target_link_libraries(${test_name} ${GTEST_BOTH_LIBRARIES} polynomial)

add_test(
    Name ${test_name}
    COMMAND ${test_name}
)
