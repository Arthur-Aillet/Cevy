
add_library(sparse_array
  STATIC
  sparse_array.cpp
  )

  add_library(entity
    STATIC
    Entity.cpp
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
target_link_libraries(ecs sparse_array entity world schedule)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX)
