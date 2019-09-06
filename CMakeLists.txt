cmake_minimum_required(VERSION 3.6)

## Command line parameter for build variant
option(ENABLE_TESTING "Configure a build tree for testing instead of normal application mode" OFF)
option(ENABLE_STATIC_CHECKS "Configure a build tree for static code analysis" OFF)

## Include config file if it exists
include(default_config.cmake OPTIONAL)

## If no external toolchain is specified and we are not building just the unit tests, use the default arm toolchain.
## Should be included as early as possible, before any project specification.
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT ${ENABLE_TESTING})
   include(toolchain_arm_gcc.cmake)
endif()

message("------------- KISO CONFIG -------------")
message("Building Kiso tests:   ${ENABLE_TESTING}")
message("Building Kiso board:   ${KISO_BOARD_NAME}")
message("Building Kiso type:    ${KISO_TYPE}")
message("Building Kiso OS:      ${KISO_OS_LIB}")
message("Building Kiso Example: ${KISO_EXAMPLE}")
message("------------- KISO CONFIG -------------")

project (Kiso C)

## Set basic project compile options
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS false)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

## Needs to be in project scope before board is loaded
if(${ENABLE_TESTING})
   enable_testing()

   # Add some warnings and debug symbols in unit tests
   add_compile_options(-O0 -g -Wall -Wextra) #-Wpedantic -fprofile-arcs -ftest-coverage --coverage
endif()

## Check for valid board and include the configuration
if(NOT DEFINED KISO_BOARD_NAME)
   message(SEND_ERROR "KISO_BOARD_NAME is a required parameter! Use cmake <...> -DKISO_BOARD_NAME=... to specify.")
elseif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/boards/${KISO_BOARD_NAME}) # Use full path - undefined behavior with relative path
   message(SEND_ERROR "board_config.cmake missing for board ${KISO_BOARD_NAME}")
else()
   include(boards/${KISO_BOARD_NAME}/board_config.cmake)
endif()

## Add board and core libs
add_subdirectory(boards)
add_subdirectory(core/essentials)
add_subdirectory(core/utils)
add_subdirectory(core/connectivity)

## Add thirdparty libs
add_subdirectory(thirdparty)

## Add main application code
add_subdirectory(examples/${KISO_BOARD_NAME}/${KISO_EXAMPLE})

## Add the documentation (not built by default target)
add_subdirectory(docs)
