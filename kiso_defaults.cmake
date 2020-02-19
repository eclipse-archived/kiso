
## Default configuration for the project.

## This file sets the defaults in case some variable is not provided as a
## command line argument to CMake invocation.

## Optionally use custom compiler for unit tests build instead of using the
## default for the Host
if(${ENABLE_TESTING})
    # set(CMAKE_C_COMPILER /usr/local/bin/gcc)
    # set(CMAKE_CXX_COMPILER /usr/local/bin/g++)
endif()

## Toolchain prefix to search for in application builds if not provided by
## command line argument. Default is 'arm-none-eabi-'.
if(NOT ARM_TOOLCHAIN_PREFIX)
    # set(ARM_TOOLCHAIN_PREFIX arm-none-eabi-)
endif()

## Provide path where ${ARM_TOOLCHAIN_PREFIX}gcc executable is located instead
## of using the one in PATH.
# set(ARM_TOOLCHAIN_DIR /usr/local/arm-gcc/bin)

## Determine KISO_BOARD_PATH, which should point to a folder containing a
## CMakeLists.txt defining targets to build the BSP. Refer to predefined boards
## from '<kiso-root>/boards/...' for reference.
##
## Users can overwrite KISO_BOARD_PATH to point to their user-defined board
## implementations. If KISO_BOARD_PATH is not set externally, assume the user
## wants to use a predefined board. Use KISO_BOARD_NAME to determine which
## predefined board should be compiled/linked.
if(KISO_BOARD_PATH)
    if(KISO_BOARD_NAME)
        ## By convention KISO_BOARD_NAME should reflect the basename of
        ## KISO_BOARD_PATH. If there's a mismatch it could cause confusion
        ## later. Read the explanation above to learn more about the relation of
        ## KISO_BOARD_NAME and KISO_BOARD_PATH.
        message(WARNING "KISO_BOARD_NAME set despite using custom KISO_BOARD_PATH.")
    else()
        ## Determine board name based on basename of KISO_BOARD_PATH.
        if (NOT IS_ABSOLUTE ${KISO_BOARD_PATH})
            message(FATAL_ERROR "KISO_BOARD_PATH must be an absolute path.")
        endif()
        get_filename_component(KISO_BOARD_NAME ${KISO_BOARD_PATH} NAME)
   endif()
else()
    if(NOT KISO_BOARD_NAME)
        ## Default board if nothing is specified.
        set(KISO_BOARD_NAME CommonGateway)
    endif()
    ## Assume user wants to use one of the predefined boards.
    set(KISO_BOARD_PATH boards/${KISO_BOARD_NAME})
endif()

## Determine OS to use (only option is freertos for now).
if(NOT KISO_OS_LIB)
    set(KISO_OS_LIB freertos)
endif()

## Determine KISO_APPLICATION_PATH, which should point to a folder containing a
## CMakeLists.txt defining targets to build the desired app. This may be an
## example app from '<kiso-root>/examples/...' or a user-defined app.
if(KISO_APPLICATION_PATH)
    if(KISO_APPLICATION_NAME)
        ## By convention KISO_APPLICATION_NAME should reflect the basename of
        ## KISO_APPLICATION_PATH. If there's a mismatch it could cause confusion
        ## later. Read the explanation above to learn more about the relation of
        ## KISO_APPLICATION_NAME and KISO_APPLICATION_PATH.
        message(WARNING "KISO_APPLICATION_NAME set despite using custom KISO_APPLICATION_PATH. KISO_APPLICATION_NAME=${KISO_APPLICATION_NAME}")
    else()
        ## Determine app name based on basename of KISO_APPLICATION_PATH.
        if (NOT IS_ABSOLUTE ${KISO_APPLICATION_PATH})
            message(FATAL_ERROR "KISO_APPLICATION_PATH must be an absolute path. You can use tricks like '-D KISO_APPLICATION_PATH=\"$(pwd)/${KISO_APPLICATION_PATH}\"' to make relative paths absolute to your invoking shells' working directory.")
        endif()
        get_filename_component(KISO_APPLICATION_NAME ${KISO_APPLICATION_PATH} NAME)
    endif()
else()
    if(NOT KISO_APPLICATION_NAME)
        ## Default example app if nothing is specified.
        set(KISO_APPLICATION_NAME c-leds)
    endif()
    ## Assume user wants to use one of the examples.
    set(KISO_APPLICATION_PATH examples/${KISO_BOARD_NAME}/${KISO_APPLICATION_NAME})
endif()

## Directory with configuration headers for packages.
if(NOT PROJECT_CONFIG_PATH)
    set(PROJECT_CONFIG_PATH ${CMAKE_SOURCE_DIR}/config)
endif()
