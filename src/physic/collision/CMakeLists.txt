##
## Agartha-Software, 2024
## Cevy
## File description:
## collision-cmake
##

add_library(collision
    STATIC
    Collider.hpp
    Collider.cpp
    Shape.hpp
    CollidingEntities.hpp
)

target_include_directories(collision PUBLIC
  ../../ecs
  ../../engine
  .
  ..
  ${RAYLIB_FOLDER}
  )