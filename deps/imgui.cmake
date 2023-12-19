FetchContent_Populate(imgui
  URL https://github.com/ocornut/imgui/archive/docking.zip
  SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/imgui
)

set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(OpenGL 2 REQUIRED)
FIND_LIBRARY(Glfw3 glfw3 PATHS "C:/Program Files/glfw/lib-vc2022")

set(IMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/imgui)

add_library(imgui STATIC
  ${IMGUI_FOLDER}/imgui.cpp
  ${IMGUI_FOLDER}/imgui.h
  ${IMGUI_FOLDER}/imgui_draw.cpp
  ${IMGUI_FOLDER}/imgui_demo.cpp
  ${IMGUI_FOLDER}/imgui_tables.cpp
  ${IMGUI_FOLDER}/imgui_widgets.cpp
)

set(RLIMGUI_FOLDER ${CMAKE_BINARY_DIR}/_deps/rlimgui)

target_include_directories(imgui
  PUBLIC
  ${IMGUI_FOLDER}
  ${IMGUI_FOLDER}/backends
)

target_link_libraries(imgui PUBLIC Glfw ${OPENGL_LIBRARIES})
