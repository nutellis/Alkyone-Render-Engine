cmake_minimum_required(VERSION 4.1)
project(AlkyoneRenderEngine)

set(CMAKE_CXX_STANDARD 20)

# GLFW Setup-----------------------------------------

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw-3.4)
# ---------------------------------------------------


add_executable(AlkyoneRenderer main.cpp)

target_link_libraries(AlkyoneRenderer PRIVATE glfw)