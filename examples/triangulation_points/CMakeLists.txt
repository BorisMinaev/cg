cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(triangulation-point-example)

add_executable(triangulation-point triangulation.cpp)
target_link_libraries(triangulation-point visualization ${GMP_LIBRARIES})
