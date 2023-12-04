add_library(entity
    STATIC
    sparse_array.cpp
    sparse_array.hpp
    entity.cpp
  )

add_library(sparse_array
  STATIC
  sparse_array.cpp
  )

add_library(app
  STATIC
  App.cpp
  App.hpp
  )

add_subdirectory(game_engine)

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../public/*")
add_library(cevy SHARED ${CEVY_HEADERS})
target_link_libraries(cevy entity app sparse_array game_engine)
set_target_properties(cevy PROPERTIES LINKER_LANGUAGE CXX)
