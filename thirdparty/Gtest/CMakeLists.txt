cmake_minimum_required(VERSION 3.6)

project(GTest CXX)

set(GTEST_DIR v2.0.0-0.2.0)
add_library(gtest STATIC
   ${GTEST_DIR}/gtest-all.cc
   ${GTEST_DIR}/gtest_main.cc
)
target_include_directories(gtest
PUBLIC
   ${GTEST_DIR}
)
# Maybe use FindPackage
target_link_libraries(gtest pthread)
