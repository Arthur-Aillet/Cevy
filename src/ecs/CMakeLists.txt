##
## Agartha-Software, 2023
## Cevy
## File description:
## ecs-cmake
##

set(CMAKE_CXX_STANDARD 17)

add_library(SparseVector
  STATIC
  SparseVector.cpp
  )

add_library(entity
  STATIC
  Entity.cpp
  Entity.hpp
  )

target_link_libraries(entity SparseVector)

include_directories(PUBLIC . ./commands ../)

add_subdirectory(commands)

add_subdirectory(input)

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

add_library(default_plugin
  STATIC
  DefaultPlugin.cpp
  DefaultPlugin.hpp
  )

set(RAYLIB_FOLDER ${CMAKE_BINARY_DIR}/_deps/raylib-build/raylib/include)
target_link_libraries(default_plugin input)

add_library(time
  STATIC
  Time.cpp
  Time.hpp
  )

add_library(app
  STATIC
  App.cpp
  App.hpp
  Plugin.hpp
  )

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../../public/*")
add_library(ecs SHARED ${CEVY_HEADERS})
target_link_libraries(ecs SparseVector world commands input app entity schedule default_plugin time)
target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/commands)
#target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/input)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
