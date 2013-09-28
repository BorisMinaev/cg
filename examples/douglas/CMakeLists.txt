cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(douglas-example)

add_executable(douglas douglas.cpp)
target_link_libraries(douglas visualization ${GMP_LIBRARIES})