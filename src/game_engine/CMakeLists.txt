add_library(game_engine
  STATIC
  position.hpp
  velocity.hpp
  game_engine.hpp
  game_engine.cpp
  )

target_link_libraries(game_engine PUBLIC raylib)
