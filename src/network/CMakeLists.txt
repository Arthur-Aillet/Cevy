##
## Agartha-Software, 2023
## Cevy
## File description:
## network cmakelist
##

add_library(network
  STATIC
  NetworkPlugin.cpp
  # network.cpp
)

set(ASIO_FOLDER ${CMAKE_BINARY_DIR}/_deps/asio)

target_link_libraries(network PUBLIC asio SparseVector ecs)


target_include_directories(network PUBLIC
  ../ecs
  ../ecs/commands
  .
)
