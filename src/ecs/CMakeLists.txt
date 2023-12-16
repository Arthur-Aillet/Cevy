##
## Agartha-Software, 2023
## Cevy
## File description:
## ecs-cmake
##


add_library(SparseVector
  STATIC
  SparseVector.cpp
  )

add_library(entity
  STATIC
  Entity.cpp
  )

target_link_libraries(entity SparseVector)

add_library(world
  STATIC
  World.cpp
  World.hpp
  Commands.cpp
  Commands.hpp
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
target_link_libraries(ecs SparseVector world entity schedule app)
target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
