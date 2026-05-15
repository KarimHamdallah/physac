cmake_minimum_required(VERSION 3.25)

project(physac)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Raylib options
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(dep/raylib)

add_executable(physac
    src/main.cpp
    src/Renderer.h
    src/physac/math/vec2.h
    src/physac/particle.h
    src/physac/verletparticle.h
    src/physac/body.h
)

target_link_libraries(physac PRIVATE raylib)

target_include_directories(physac PRIVATE
    src
    dep/raylib/src
)