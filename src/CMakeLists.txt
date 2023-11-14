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

add_library(registery
  STATIC
  registery.cpp
  registery.hpp
  )

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../public/*")
add_library(cevy SHARED ${CEVY_HEADERS})
target_link_libraries(cevy entity registery sparse_array)
set_target_properties(cevy PROPERTIES LINKER_LANGUAGE CXX)
