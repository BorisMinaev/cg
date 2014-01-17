cmake_minimum_required(VERSION 2.8)

project(cg-test)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

find_package(GMP REQUIRED)
include_directories(${GMP_INCLUDE_DIR})

find_package(Boost COMPONENTS random REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARYDIR})

find_package(CGAL REQUIRED)
INCLUDE_DIRECTORIES(
  #  ${CGAL_INCLUDE_DIR}
)
ADD_DEFINITIONS(${CGAL_DEFINITIONS})

set(SOURCES
   triangulation.cpp
   orientation.cpp
   has_intersection.cpp
   contains.cpp
   convex_hull.cpp
   dynamic_convex_hull.cpp
   douglas_pecker.cpp
   convex.cpp
   bentley_ottmann.cpp
   triangulation.cpp
   range_tree_test.cpp
   #cgal_convex_hull_test.cpp
)

add_executable(cg-test ${SOURCES})
target_link_libraries(cg-test ${GTEST_BOTH_LIBRARIES} ${GMP_LIBRARIES})

add_executable(bentley-ottmann bentley_ottmann.cpp)
target_link_libraries(bentley-ottmann ${GTEST_BOTH_LIBRARIES} ${GMP_LIBRARIES})

file(GLOB_RECURSE HEADERS "*.h")
add_custom_target(cg_test_headers SOURCES ${HEADERS})
