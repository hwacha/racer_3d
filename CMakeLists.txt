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

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "XXX ${_variableName}=${${_variableName}}")
endforeach()


add_executable(game main.cpp glad/glad.c)
add_dependencies(game glfw)
target_include_directories(game PUBLIC glfw-3.3.2/include glm/ assimp-5.0.0/include)
target_link_libraries(game glfw)
