file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

add_executable(test-cevy ${TEST_SOURCES})

target_link_libraries(test-cevy PUBLIC cevy)
target_link_libraries(test-cevy PUBLIC criterion)
