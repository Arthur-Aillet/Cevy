##
## Agartha-Software, 2023
## Cevy
## File description:
## math-cmake
##

add_library(math
  STATIC
  Vector.cpp
  )

target_include_directories(math PUBLIC
  .
  ..
  ${RAYLIB_FOLDER}
  )
