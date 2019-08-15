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

# TODO: Enable later - not working for now
# Need little attention
list(FILTER TEST_CODE EXCLUDE REGEX ".*SPI_unittest.cc$")
# Needs major work
list(FILTER TEST_CODE EXCLUDE REGEX ".*UART_unittest.cc$")
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
   get_filename_component(unit_name ${unittest_file} NAME_WE) # Name without extension or directory
   add_executable(essentials_${unit_name} ${unittest_file})
   add_dependencies(essentials_tests essentials_${unit_name})
   add_test(${unit_name} essentials_${unit_name})
endforeach(unittest_file ${TEST_CODE})
