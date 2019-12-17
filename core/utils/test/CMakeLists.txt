cmake_minimum_required(VERSION 3.6)

project (UtilsTests C CXX)

add_library(utils_test_int INTERFACE)
target_include_directories(utils_test_int
INTERFACE
   unit/include
)
target_link_libraries(utils_test_int INTERFACE utils_int)

file(GLOB TEST_CODE
   unit/source/*.cc
   unit/source/${KISO_OS_LIB}/*.cc
   unit/source/Logging/*.cc
   unit/source/Logging/${KISO_OS_LIB}/*.cc
)

include_directories(
   ../source
   ../source/${KISO_OS_LIB}
   ../source/Logging
   ../source/Logging/${KISO_OS_LIB}
)

if(${ENABLE_ALL_FEATURES})
   add_compile_options(
      -DKISO_FEATURE_CMDLINEDEBUGGER=1
      -DKISO_FEATURE_CMDPROCESSOR=1
      -DKISO_FEATURE_CRC=1
      -DKISO_FEATURE_EVENTHUB=1
      -DKISO_FEATURE_GUARDEDTASK=1
      -DKISO_FEATURE_ERRORLOGGER=1
      -DKISO_FEATURE_LOGGING=1
      -DKISO_FEATURE_RINGBUFFER=1
      -DKISO_FEATURE_SLEEPCONTROL=1
      -DKISO_FEATURE_TASKMONITOR=1
      -DKISO_TASKMONITOR_MAX_TASKS=10
      -DKISO_FEATURE_UARTTRANSCEIVER=1
      -DKISO_FEATURE_I2CTRANSCEIVER=1
      -DKISO_FEATURE_XPROTOCOL=1
      -DKISO_FEATURE_PIPEANDFILTER=1
   )
endif()

link_libraries(utils_test_int gtest ${KISO_OS_LIB}_test_int essentials_test_int)
foreach(lib ${KISO_BOARD_LIBS})
   link_libraries(${lib}_test_int)
endforeach(lib ${KISO_BOARD_LIBS})

add_custom_target(utils_tests)

## Build each test file as it's own executable
## Name each executable after the .cc file it's built from
## Add it to a custom target for all utils tests
## Register as test for ctest
foreach(unittest_file ${TEST_CODE})
   get_filename_component(unit_name ${unittest_file} NAME_WE)
   add_executable(utils_${unit_name} ${unittest_file})
   add_dependencies(utils_tests utils_${unit_name})
   add_test(${unit_name} utils_${unit_name})
endforeach(unittest_file ${TEST_CODE})

if(${ENABLE_COVERAGE})
   # From CodeCoverage module
   SETUP_TARGET_FOR_COVERAGE_LCOV(
      NAME utils_cov
      EXECUTABLE ${CMAKE_CTEST_COMMAND}
      DEPENDENCIES utils_tests
      LCOV_ARGS --rc lcov_branch_coverage=1
      GENHTML_ARGS --branch-coverage
   )
   add_dependencies(coverage utils_cov)
endif()
