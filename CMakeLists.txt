cmake_minimum_required(VERSION 3.5)
project(cppmc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(cppmc_sources
  cppmc/camera.cpp
  cppmc/cube_mesh.cpp
  cppmc/mesh.cpp
  cppmc/shader.cpp
  cppmc/texture_atlas.cpp
  cppmc/util.cpp
  cppmc/world.cpp
  cppmc/world_common.cpp
  main.cpp
  )

list(TRANSFORM cppmc_sources PREPEND src/)

add_executable(cppmc)
target_sources(cppmc PRIVATE ${cppmc_sources})
target_include_directories(cppmc
  PRIVATE src
  PRIVATE vendors
  )
target_link_libraries(cppmc
  PRIVATE glfw
  PRIVATE GLEW
  PRIVATE GL
  )