cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(bentley_ottmann-example2)

add_executable(bentley_ottmann-vizualizator bentley_ottmann.cpp)
target_link_libraries(bentley_ottmann-vizualizator visualization ${GMP_LIBRARIES})
