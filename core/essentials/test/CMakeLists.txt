cmake_minimum_required(VERSION 3.6)

project (EssentialsTests)

add_library(essentials_test_int INTERFACE)
target_include_directories(essentials_test_int
INTERFACE
   unit/include
   unit/include/bsp
   unit/include/mcu
)
target_link_libraries(essentials_test_int INTERFACE essentials_int)

file(GLOB TEST_CODE
   unit/source/*.cc
   unit/source/${KISO_MCU_FAMILY}/*.cc
   unit/source/${KISO_MCU_FAMILY}/${KISO_MCU_SERIES}/*.cc
)

# this is a workaround of not treat int-to-pointer-caster as an error
# essential unit test might rethink about a better way to do this
add_compile_options(-Wno-error=int-to-pointer-cast)

if(${ENABLE_ALL_FEATURES})
   # TODO: Enable watchdog feature once unit-tests are compilable
   add_compile_options(
      -DKISO_FEATURE_UART=1
      -DKISO_FEATURE_I2C=1
      -DKISO_FEATURE_SPI=1
      -DKISO_FEATURE_FLASH=1
      -DKISO_FEATURE_WATCHDOG=0
      -DKISO_FEATURE_PWM=1
      -DKISO_FEATURE_SLEEP=1
      -DKISO_FEATURE_RNG=1
      -DKISO_FEATURE_MCU_CRC=1
      -DKISO_FEATURE_BSP_LED=1
      -DKISO_FEATURE_BSP_BUTTON=1
      -DKISO_FEATURE_BSP_CELLULAR_SARAR4N4=1
      -DKISO_FEATURE_BSP_GNSS_MAXM8=1
      -DKISO_FEATURE_BSP_BMA280=1
      -DKISO_FEATURE_BSP_BME280=1
      -DKISO_FEATURE_BSP_BLE_CC2640=1
      -DKISO_FEATURE_BSP_MEMORY_W25=1
      -DKISO_FEATURE_BSP_TEST_INTERFACE=1
   )
endif()

# TODO: Enable later - not working for now
# Unknown
list(FILTER TEST_CODE EXCLUDE REGEX ".*efm32/Flash_unittest.cc$")
list(FILTER TEST_CODE EXCLUDE REGEX ".*efm32/Timer_unittest.cc$")
list(FILTER TEST_CODE EXCLUDE REGEX ".*efm32/Sleep_unittest.cc$")

# Valid for all tragets in current project
include_directories(
   ../source
   ../source/mcu/${KISO_MCU_FAMILY}
   ../source/mcu/${KISO_MCU_FAMILY}/${KISO_MCU_SERIES}
)
link_libraries(essentials_test_int gtest)
foreach(lib ${KISO_BOARD_LIBS})
   link_libraries(${lib}_test_int)
endforeach(lib ${KISO_BOARD_LIBS})

# Artificial target to depend on all tests (allows building of all tests for essentials only with one target)
add_custom_target(essentials_tests)

## Build each test file as it's own executable
## Name each executable after the .cc file it's built from
## Add it to a custom target for all essentials tests
## Register as test for ctest
foreach(unittest_file ${TEST_CODE})
   get_filename_component(unit_name ${unittest_file} NAME_WE)
   add_executable(essentials_${unit_name} ${unittest_file})
   add_dependencies(essentials_tests essentials_${unit_name})
   add_test(${unit_name} essentials_${unit_name})
endforeach(unittest_file ${TEST_CODE})

if(${ENABLE_COVERAGE})
   # From CodeCoverage module
   SETUP_TARGET_FOR_COVERAGE_LCOV(
      NAME essentials_cov
      EXECUTABLE ${CMAKE_CTEST_COMMAND}
      DEPENDENCIES essentials_tests
      LCOV_ARGS --rc lcov_branch_coverage=1
      GENHTML_ARGS --branch-coverage
   )
   add_dependencies(coverage essentials_cov)
endif()
