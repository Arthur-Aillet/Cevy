cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)

project(Cevy LANGUAGES CXX VERSION 0.1)
set(Cevy_VERSION 0.1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/conan/conan_toolchain.cmake")

add_subdirectory(deps)

add_subdirectory(src)

option(TESTS "Build tests" OFF)
if(TESTS)
    add_custom_target(tests-run-cevy
                        COMMENT "Executing tests"
                        COMMAND test-cevy
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        SOURCES ${ToolsSources}
                    )

    add_subdirectory(test)
endif()
