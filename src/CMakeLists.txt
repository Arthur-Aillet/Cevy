add_library(entity.a
    STATIC
    sparse_array.cpp
    sparse_array.hpp
    entity.cpp
  )

add_library(sparse_array.a
  STATIC
  sparse_array.cpp
  )

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../public/*")
add_library(cevy SHARED ${CEVY_HEADERS})
target_link_libraries(cevy entity.a)
set_target_properties(cevy PROPERTIES LINKER_LANGUAGE CXX)
