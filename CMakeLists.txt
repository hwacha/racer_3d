project(racer_3d)
cmake_minimum_required(VERSION 3.10)

# Currently this CMakeLists works on windows, but with glfw-3.3.2,
# glm, and assimp-5.0.0 vendored into subdirectories. TODO we can make
# it be cross platform and use system packages with find_package.

find_package(OpenGL REQUIRED)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.3.2)
add_subdirectory(glm)
add_subdirectory(assimp-5.0.0)

add_executable(game main.cpp skybox.cpp glad/glad.c)
add_dependencies(game glfw assimp)
target_include_directories(game PUBLIC glfw-3.3.2/include glm/ "${Assimp_SOURCE_DIR}/include" "${Assimp_BINARY_DIR}/include")
target_link_libraries(game glfw assimp)
