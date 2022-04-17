add_library(application application.cpp)
target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(application PRIVATE polynomial)