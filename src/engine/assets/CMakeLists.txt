##
## Agartha-Software, 2023
## Cevy
## File description:
## assets-cmake
##

add_library(assets
  STATIC
  AssetManager.cpp
  Asset.hpp
  Handle.hpp
  AssetManager.hpp
  )

target_include_directories(assets PRIVATE ${RAYLIB_FOLDER})

target_include_directories(assets PUBLIC
  ../../ecs
  .
  ..
  )
