cmake_minimum_required(VERSION 3.6)

option(ENABLE_TESTING "Configure a build tree for testing instead of normal application mode" OFF)

# If no external toolchain is specified and we are not building just the unit tests, use the default arm toolchain.
# Should be included as early as possible, before any project specification.
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT ${ENABLE_TESTING})
   include(toolchain_arm_gcc.cmake)
endif()

project (Kiso C)

# Set basic project compile options
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS false)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

# Kiso variant
set(KISO_TYPE Carbon)

# OS to use (only option is freertos for now)
set(KISO_OS_LIB freertos)

# Needs to be in project scope before board is loaded
if(${ENABLE_TESTING})
   enable_testing()
endif()

# Check for valid board and include the configuration
if(NOT DEFINED KISO_BOARD_NAME)
   message(SEND_ERROR "KISO_BOARD_NAME is a required parameter! Use cmake <...> -D KISO_BOARD_NAME=... to specify.")
elseif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/boards/${KISO_BOARD_NAME}) # Use full path - undefined behavior with relative path
   message(SEND_ERROR "board_config.cmake missing for board ${KISO_BOARD_NAME}")
else()
   include(boards/${KISO_BOARD_NAME}/board_config.cmake)
endif()

# Add board and core libs
add_subdirectory(boards)
add_subdirectory(core/essentials)
add_subdirectory(core/utils)
add_subdirectory(core/connectivity)

# Add thirdparty libs
add_subdirectory(thirdparty)

# Add main application code
add_subdirectory(examples/${KISO_BOARD_NAME}/c-leds)

# Add the documentation (not built by default target)
add_subdirectory(docs/doxygen)
