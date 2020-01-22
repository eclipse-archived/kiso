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

find_program(WSLPATH NAMES wslpath)

if(NOT WSLPATH) # Linux or Windows
    find_program(JLINK_PATH NAMES JLinkExe jlink jlink.exe)
else() # WSL - find only WIN binaries
    find_program(JLINK_PATH NAMES jlink jlink.exe)
endif()

if(NOT JLINK_PATH)
    message(STATUS "JLink not found in PATH. \"flash\" target unavailable!")
else()
    message(STATUS "JLink found: ${JLINK_PATH}. Use target 'flash' to flash connected device.")
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
    if(NOT JLINK_PATH)
        message(ERROR "JLink not found in path!")
    endif()

    set(HEX_TARGET_PATH "${CMAKE_CURRENT_BINARY_DIR}/${ELF_TARGET}.hex")

    add_custom_target(${ELF_TARGET}.hex
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${ELF_TARGET}> ${HEX_TARGET_PATH}
        COMMENT "Creating flashable hex ${HEX_TARGET_PATH}"
    )
    add_dependencies(${ELF_TARGET}.hex ${ELF_TARGET})

    set(SCRIPT_PATH ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink)

    # WSL workaround
    # WSL does not expose USB devices so native linux jlink binaries can't be used.
    # Patch the paths with the provided wslpath utility so that WIN jlink can recognize them outside WSL
    if(WSLPATH)
        # 'wslpath -w' to be read directly inside jlink
        execute_process(COMMAND ${WSLPATH} -w ${HEX_TARGET_PATH} OUTPUT_VARIABLE WIN_TARGET_PATH)
        string(STRIP ${WIN_TARGET_PATH} HEX_TARGET_PATH)
    endif()

    # \todo: This can be exported to file and replaced with configure_file
    file(WRITE  ${SCRIPT_PATH} "exitonerror 1\n")
    file(APPEND ${SCRIPT_PATH} "device ${KISO_MCU_DEVICE}\n")
    file(APPEND ${SCRIPT_PATH} "if swd\n")
    file(APPEND ${SCRIPT_PATH} "speed 4000\n")
    file(APPEND ${SCRIPT_PATH} "connect\n")
    file(APPEND ${SCRIPT_PATH} "r\n")
    file(APPEND ${SCRIPT_PATH} "h\n")
    file(APPEND ${SCRIPT_PATH} "loadfile ${HEX_TARGET_PATH}\n")
    file(APPEND ${SCRIPT_PATH} "r\n")
    file(APPEND ${SCRIPT_PATH} "qc\n")

    if(WSLPATH)
        # 'wslpath -m' so it's not escaped improperly
        execute_process(COMMAND ${WSLPATH} -m ${SCRIPT_PATH} OUTPUT_VARIABLE WIN_SCRIPT_PATH)
        string(STRIP ${WIN_SCRIPT_PATH} SCRIPT_PATH)
    endif()

    add_custom_target(flash
        COMMAND ${JLINK_PATH} -CommanderScript ${SCRIPT_PATH}
    )
    add_dependencies(flash ${ELF_TARGET}.hex)
endfunction(CREATE_FLASH_TARGET_JLINK)

# Used to create a raw binary target from a given ELF target
# that could later be used to flash the application directly to a memory address
# Usage:
#  In CMake:
#    ```
#    add_executable(application
#        source/main.c
#    )
#    CREATE_FLASHABLE_BINARY(application)
#    ```
#  In console:
#    cmake --build . --target application.bin
function(CREATE_FLASHABLE_BINARY ELF_TARGET)
    set(BIN_TARGET_PATH "${CMAKE_CURRENT_BINARY_DIR}/${ELF_TARGET}.bin")

    add_custom_target(${ELF_TARGET}.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary -R .usrpg $<TARGET_FILE:${ELF_TARGET}> ${BIN_TARGET_PATH}
        COMMENT "Creating flashable binary ${BIN_TARGET_PATH}"
    )
    add_dependencies(${ELF_TARGET}.bin ${ELF_TARGET})
endfunction(CREATE_FLASHABLE_BINARY ELF_TARGET)
