cmake_minimum_required(VERSION 3.2)
project(main)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
# Define executable target
add_executable(main main.cpp compileshaders.cpp mesh.cpp camera.cpp)
target_link_libraries(main glfw)
target_link_libraries(main OpenGL::GL)

