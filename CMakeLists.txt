cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

project(Cevy VERSION 0.1)
set(Cevy_VERSION 0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_subdirectory(src)

add_executable(test main.cpp)

target_link_libraries(test PUBLIC test.a)

#add_library(cevy SHARED main.c)
