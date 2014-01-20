cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(intersect-sphere-example)

add_executable(intersect_sphere intersect_sphere.cpp)
target_link_libraries(intersect_sphere visualization ${GMP_LIBRARIES})
