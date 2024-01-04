##
## Agartha-Software, 2024
## Cevy
## File description:
## physic-cmake
##

add_library(physic
    SHARED
    physic.hpp
  )

set(RLIMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/rlimgui)
set(IMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/imgui)
set(RAYLIB_FOLDER ${CMAKE_BINARY_DIR}/_deps/raylib-build/raylib/include)

add_subdirectory(collision)

target_link_libraries(physic PUBLIC raylib imgui rlImGui ecs engine collision)

target_include_directories(physic PUBLIC
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
