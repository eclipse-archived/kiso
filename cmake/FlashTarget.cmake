################################################################################
# Copyright (c) 2010-2019 Robert Bosch GmbH
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Robert Bosch GmbH - initial contribution
#
################################################################################

cmake_minimum_required(VERSION 3.6)

set(JLINK_FOUND FALSE)
find_program(JLINK_PATH NAMES jlink jlink.exe)

if(NOT JLINK_PATH)
    set(JLINK_FOUND TRUE)
    message(STATUS "JLink not found in PATH. \"flash\" target unavailable!")
else()
    message(STATUS "JLink found: ${JLINK_PATH}")
endif()

# Used to create an intel hex target from a given ELF target
# and a 'flash' target that uses the ihex and a JLink script to flash the application to a device
# Usage:
#  In CMake:
#    ```
#    add_executable(application
#        source/main.c
#    )
#    CREATE_FLASH_TARGET_JLINK(application)
#    ```
#  In console:
#    cmake --build . --target flash
#
function(CREATE_FLASH_TARGET_JLINK ELF_TARGET)
    add_custom_target(${ELF_TARGET}.hex
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${ELF_TARGET}> ${CMAKE_CURRENT_BINARY_DIR}/${ELF_TARGET}.hex
        COMMENT "Creating flashable hex ${CMAKE_CURRENT_BINARY_DIR}/${ELF_TARGET}.hex"
    )
    add_dependencies(${ELF_TARGET}.hex ${ELF_TARGET})

    # \todo: This can be exported to file and replaced with configure_file
    file(WRITE  ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "exitonerror 1\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "device ${KISO_MCU_DEVICE}\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "if swd\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "speed 4000\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "connect\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "r\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "h\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "loadfile ${CMAKE_CURRENT_BINARY_DIR}/${ELF_TARGET}.hex\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "r\n")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink "qc\n")

    add_custom_target(flash
        COMMAND ${JLINK_PATH} -CommanderScript ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink
    )
    add_dependencies(flash ${ELF_TARGET}.hex)
endfunction(CREATE_FLASH_TARGET_JLINK)
