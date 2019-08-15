cmake_minimum_required(VERSION 3.6)

project(GTest C CXX)

set(GTEST_DIR v2.0.0-0.2.0)
add_library(gtest STATIC
   ${GTEST_DIR}/gtest-all.cc
   ${GTEST_DIR}/gtest_main.cc
)
target_include_directories(gtest PUBLIC
   ${GTEST_DIR}
)

find_package(Threads REQUIRED)
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})
