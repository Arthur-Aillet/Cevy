##
## Agartha-Software, 2023
## Cevy
## File description:
## ecs-cmake
##


add_library(sparse_array
  STATIC
  sparse_array.cpp
  )

add_library(entity
  STATIC
  Entity.cpp
  )

add_library(zipper
  STATIC
  zipper_iterator.hpp
  zipper_iterator.cpp
  )

target_link_libraries(entity sparse_array)

add_library(world
  STATIC
  World.cpp
  World.hpp
  )

add_library(schedule
  STATIC
  Schedule.cpp
  Schedule.hpp
  )

add_library(app
  STATIC
  App.cpp
  App.hpp
  Plugin.hpp
  )

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../../public/*")
add_library(ecs SHARED ${CEVY_HEADERS})
target_link_libraries(ecs sparse_array entity world schedule app zipper)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
