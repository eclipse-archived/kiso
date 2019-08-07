cmake_minimum_required(VERSION 3.6)

project(CellularTests C CXX)

add_library(cellular_test_int INTERFACE)
target_include_directories(cellular_test_int
INTERFACE
   unit/include
)
target_link_libraries(cellular_test_int INTERFACE cellular_int)

file(GLOB TEST_CODE
   unit/source/*_unittest.cc
   unit/source/common/*_unittest.cc
   unit/source/ublox/*_unittest.cc
)

# Valid for all tragets in current project
include_directories(
   ../source
   ../source/common
   ../source/protected
   ../source/ublox
   unit/source
   unit/source/common
   unit/source/protected
   unit/source/ublox
)
link_libraries(cellular_test_int gtest ${KISO_OS_LIB}_test_int essentials_test_int utils_test_int)
foreach(lib ${KISO_BOARD_LIBS})
   link_libraries(${lib}_test_int)
endforeach(lib ${KISO_BOARD_LIBS})

# Artificial target to depend on all tests (allows building of all tests for cellular only with one target)
add_custom_target(cellular_tests)

## Build each test file as it's own executable
## Name each executable after the .cc file it's built from
## Add it to a custom target for all cellular tests
## Register as test for ctest
foreach(unittest_file ${TEST_CODE})
   get_filename_component(unit_name ${unittest_file} NAME_WE) # Name without extension or directory
   add_executable(cellular_${unit_name} ${unittest_file})
   add_dependencies(cellular_tests cellular_${unit_name})
   add_test(${unit_name} cellular_${unit_name})
endforeach(unittest_file ${TEST_CODE})
