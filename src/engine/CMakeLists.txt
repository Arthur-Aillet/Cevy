##
## Agartha-Software, 2023
## Cevy
## File description:
## engine-cmake
##

set(CMAKE_CXX_STANDARD 17)

add_library(engine
  SHARED
  Position.hpp
  velocity.hpp
  Engine.hpp
  Engine.cpp
  Camera.hpp
  Camera.cpp
)

set(RLIMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/rlimgui)
set(IMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/imgui)
set(RAYLIB_FOLDER ${CMAKE_BINARY_DIR}/_deps/raylib-build/raylib/include)

add_subdirectory(assets)
add_subdirectory(rendering)

target_link_libraries(engine PUBLIC raylib imgui rlImGui ecs assets rendering)

target_include_directories(engine PUBLIC
  ../ecs
  ../ecs/commands
  .
  assets
  rendering
)

target_include_directories(rlImGui PUBLIC
  ${RLIMGUI_FOLDER}
  ${IMGUI_FOLDER}
  ${IMGUI_FOLDER}/backends
)
