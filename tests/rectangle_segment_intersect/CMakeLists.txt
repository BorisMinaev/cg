cmake_minimum_required(VERSION 2.8)

project(rectangle-segment-intersect-test)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/Modules)

find_package(GMP REQUIRED)
include_directories(${GMP_INCLUDE_DIR})

find_package(Boost COMPONENTS random REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} ../../include)
link_directories(${Boost_LIBRARYDIR})

set(SOURCES
   rectangle_segment_intersect_test.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} ${GMP_LIBRARIES})

file(GLOB_RECURSE HEADERS "*.h")
add_custom_target(cg_test_headers SOURCES ${HEADERS})

# google test
enable_testing()
find_package (Threads)
message(STATUS GTEST_ROOT=$ENV{GTEST_ROOT})
include_directories($ENV{GTEST_ROOT}/include)
link_directories($ENV{GTEST_ROOT}/mybuild)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
