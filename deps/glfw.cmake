set(GLFW_VERSION 3.3.9)
find_package(glfw3 QUIET)
if (NOT glfw3_FOUND) # If there's none, fetch and build glfw3
  FetchContent_Declare(
    glfw3
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/glfw/glfw/archive/refs/tags/${GLFW_VERSION}.tar.gz
  )
  FetchContent_GetProperties(glfw3)
  if (NOT glfw3_POPULATED) # Have we downloaded glfw3 yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(glfw3)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
  endif()
endif()
