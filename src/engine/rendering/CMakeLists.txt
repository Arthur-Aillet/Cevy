##
## Agartha-Software, 2023
## Cevy
## File description:
## rendering-cmake
##

add_library(rendering
  STATIC
  rendering.cpp
  Color.cpp
  )

target_include_directories(rendering PUBLIC
  ../../ecs
  ../assets
  ../math
  .
  ..
  ${RAYLIB_FOLDER}
  )
