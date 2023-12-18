##
## Agartha-Software, 2023
## Cevy
## File description:
## cevy-cmake
##

include_directories(".")

add_subdirectory(game_engine)
add_subdirectory(network)
add_subdirectory(ecs)

file(GLOB CEVY_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "../public/*")
add_library(cevy SHARED ${CEVY_HEADERS})
target_link_libraries(cevy PUBLIC ecs game_engine)
set_target_properties(cevy PROPERTIES LINKER_LANGUAGE CXX)
