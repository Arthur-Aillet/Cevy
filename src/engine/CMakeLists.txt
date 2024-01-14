##
## Agartha-Software, 2023
## Cevy
## File description:
## engine-cmake
##

set(CMAKE_CXX_STANDARD 17)

add_library(engine
  Engine.cpp
  Camera.cpp
)

set(RLIMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/rlimgui)
set(IMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/imgui)
set(RAYLIB_FOLDER ${CMAKE_BINARY_DIR}/_deps/raylib-build/raylib/include)

add_subdirectory(math)
add_subdirectory(assets)
add_subdirectory(input)
add_subdirectory(rendering)

target_link_libraries(engine PUBLIC raylib imgui rlImGui) #PRIVATE
target_link_libraries(engine PUBLIC ecs math assets rendering input)

target_include_directories(engine PUBLIC
  ../ecs
  ../ecs/commands
  .
  ./math
  assets
  rendering
  input
)

target_include_directories(rlImGui PRIVATE
  ${RLIMGUI_FOLDER}
  ${IMGUI_FOLDER}
  ${IMGUI_FOLDER}/backends
)
