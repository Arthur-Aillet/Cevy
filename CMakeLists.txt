cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

project(Cevy LANGUAGES CXX VERSION 0.1)
set(Cevy_VERSION 0.1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

file(GLOB CEVY_HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "includes/*")

add_subdirectory(src)
add_library(cevy SHARED ${CEVY_HEADERS})
target_link_libraries(cevy entity.a)
set_target_properties(cevy PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
add_executable(test ${TEST_SOURCES})
target_link_libraries(test cevy)
target_link_libraries(test criterion)
