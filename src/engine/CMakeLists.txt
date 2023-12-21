##
## Agartha-Software, 2023
## Cevy
## File description:
## engine-cmake
##

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

target_link_libraries(engine PUBLIC raylib imgui rlImGui ecs assets)

target_include_directories(engine PUBLIC
  ../ecs
  .
  assets
)

target_include_directories(rlImGui PUBLIC
  ${RLIMGUI_FOLDER}
  ${IMGUI_FOLDER}
  ${IMGUI_FOLDER}/backends
)
