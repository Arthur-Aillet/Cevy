add_library(network
  STATIC
  client.hpp
  client.cpp
  server.hpp
  server.cpp
  network.hpp
  network.cpp
)

set(ASIO_FOLDER ${CMAKE_BINARY_DIR}/_deps/asio)

target_link_libraries(network PUBLIC asio)