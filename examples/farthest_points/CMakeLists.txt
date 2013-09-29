cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(farthest_points-example2)

add_executable(farthest_points-vizualizator farthest_points.cpp)
target_link_libraries(farthest_points-vizualizator visualization ${GMP_LIBRARIES})
