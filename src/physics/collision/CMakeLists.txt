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
    Shape.cpp
    EntitiesCollision.cpp
)

target_include_directories(collision PUBLIC
  ../../ecs
  ../../engine
  ../../engine/math
  .
  ..
  ${RAYLIB_FOLDER}
  )