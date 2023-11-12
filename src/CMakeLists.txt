add_library(entity.a
    STATIC
    entity.cpp
  )

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../public/*")
add_library(cevy SHARED ${CEVY_HEADERS})
target_link_libraries(cevy entity.a)
set_target_properties(cevy PROPERTIES LINKER_LANGUAGE CXX)
