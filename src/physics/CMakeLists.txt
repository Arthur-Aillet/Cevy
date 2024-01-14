##
## Agartha-Software, 2024
## Cevy
## File description:
## physics-cmake
##

add_library(physics
    SHARED
    Physics.hpp
    Physics.cpp
  )

set(RLIMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/rlimgui)
set(IMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/imgui)
set(RAYLIB_FOLDER ${CMAKE_BINARY_DIR}/_deps/raylib-build/raylib/include)

add_subdirectory(collision)

target_link_libraries(physics PUBLIC raylib imgui rlImGui ecs engine collision)

target_include_directories(physics PUBLIC
 ../ecs
 ../engine
 ../engine/math
 .
 ./collision
)

target_include_directories(rlImGui PUBLIC
 ${RLIMGUI_FOLDER}
 ${IMGUI_FOLDER}
 ${IMGUI_FOLDER}/backends
)
