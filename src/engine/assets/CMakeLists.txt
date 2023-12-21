##
## Agartha-Software, 2023
## Cevy
## File description:
## assets-cmake
##

add_library(assets
  STATIC
  AssetManager.cpp
  AssetManager.hpp
  )

target_include_directories(assets PUBLIC
  ../../ecs
  .
  ..
  ${RAYLIB_FOLDER}
  )
