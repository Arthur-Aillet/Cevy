##
## Agartha-Software, 2023
## Cevy
## File description:
## engine-cmake
##


add_library(engine
  STATIC
  Position.hpp
  velocity.hpp
  Engine.hpp
  Engine.cpp
  Camera.hpp
  Camera.cpp
  assets/AssetManager.cpp
  assets/AssetManager.hpp
  )

set(RLIMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/rlimgui)
set(IMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/imgui)

target_link_libraries(engine PUBLIC raylib imgui rlImGui ecs)

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
