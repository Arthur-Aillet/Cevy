##
## Agartha-Software, 2023
## Cevy
## File description:
## Input cmakelists
##

add_library(input
  STATIC
  Input.cpp
  Input.hpp
  Keyboard.cpp
  Keyboard.hpp
  Mouse.cpp
  )

set(RAYLIB_FOLDER ${CMAKE_BINARY_DIR}/_deps/raylib-build/raylib/include)
target_link_libraries(input raylib)

target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../ecs ${CMAKE_CURRENT_SOURCE_DIR}/..)
