cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(douglas-peucker-example)

add_executable(douglas-peucker douglas_peucker.cpp)
target_link_libraries(douglas-peucker visualization ${GMP_LIBRARIES})
