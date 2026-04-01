cmake_minimum_required(VERSION 3.16)
project(opengl_test C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

add_executable(app
  src/main.cpp
  src/renderer.cpp
  src/shader.cpp
  src/calculatePendulum.cpp
  external/glad/src/glad.c
)

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    ${CMAKE_BINARY_DIR}/shaders
)

add_dependencies(app copy_shaders)

add_library(imgui STATIC
  external/imgui/src/imgui.cpp
  external/imgui/src/imgui_draw.cpp
  external/imgui/src/imgui_tables.cpp
  external/imgui/src/imgui_widgets.cpp
  external/imgui/src/imgui_tables.cpp
  external/imgui/src/backends/imgui_impl_glfw.cpp
  external/imgui/src/backends/imgui_impl_opengl3.cpp
)

target_include_directories(app PRIVATE
  ${CMAKE_SOURCE_DIR}/header
  ${CMAKE_SOURCE_DIR}/external/glad/include
)
target_include_directories(imgui PUBLIC
  ${CMAKE_SOURCE_DIR}/external/imgui/include
  ${CMAKE_SOURCE_DIR}/external/imgui/include/backends
  ${GLFW_INCLUDE_DIRS}
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

target_include_directories(app PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${GLFW_LIBRARIES} GL dl pthread)