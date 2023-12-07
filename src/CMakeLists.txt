include_directories(".")

add_subdirectory(game_engine)
add_subdirectory(ecs)

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../public/*")
add_library(cevy SHARED ${CEVY_HEADERS})
target_link_libraries(cevy ecs game_engine)
set_target_properties(cevy PROPERTIES LINKER_LANGUAGE CXX)
