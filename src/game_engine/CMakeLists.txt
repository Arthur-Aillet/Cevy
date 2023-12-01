add_library(game_engine
  STATIC
  position.hpp
  velocity.hpp
  game_engine.hpp
  game_engine.cpp
  camera.hpp
  camera.cpp
  )

  set(RLIMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/rlimgui)
  set(IMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/imgui)

target_link_libraries(game_engine PUBLIC raylib imgui rlImGui)

message("message [${RLIMGUI_FOLDER}] [${IMGUI_FOLDER}]")

target_include_directories(rlImGui PUBLIC
  ${RLIMGUI_FOLDER}
  ${IMGUI_FOLDER}
  ${IMGUI_FOLDER}/backends
)
