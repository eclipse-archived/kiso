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

if(NOT PROJECT_CONFIG_PATH)
    message(SEND_ERROR "PROJECT_CONFIG_PATH must be set to a valid path!")
endif()

get_filename_component(ABS_PROJECT_CONFIG_PATH ${PROJECT_CONFIG_PATH} REALPATH)
if(NOT EXISTS ${ABS_PROJECT_CONFIG_PATH})
    message(SEND_ERROR "PROJECT_CONFIG_PATH must be absolute or relative to Kiso root!\n"
                       "Current value: ${ABS_PROJECT_CONFIG_PATH}")
endif()

if(NOT BOARD_CONFIG_PATH)
    message(SEND_ERROR "BOARD_CONFIG_PATH must be set to a valid path!")
endif()

get_filename_component(ABS_BOARD_CONFIG_PATH ${BOARD_CONFIG_PATH} REALPATH)
if(NOT EXISTS ${ABS_BOARD_CONFIG_PATH})
    message(SEND_ERROR "BOARD_CONFIG_PATH must be absolute or relative to Kiso root!\n"
                       "Current value: ${ABS_BOARD_CONFIG_PATH}")
endif()

# This variable is internal and managed through this file only
set(KISO_CONFIG_PATH ${CMAKE_BINARY_DIR}/config CACHE INTERNAL "Kiso config headers location")

# Copy base project config files in intermediary directory
file(GLOB_RECURSE PRJ_CONF_FILES
    LIST_DIRECTORIES false
    RELATIVE ${ABS_PROJECT_CONFIG_PATH}
    ${ABS_PROJECT_CONFIG_PATH}/*.h
)

foreach(HEADER ${PRJ_CONF_FILES})
    set(DEST ${KISO_CONFIG_PATH})
    get_filename_component(SUBDIR ${HEADER} DIRECTORY)
    if(SUBDIR)
        set(DEST ${KISO_CONFIG_PATH}/${SUBDIR})
        file(MAKE_DIRECTORY ${DEST})
    endif(SUBDIR)
    configure_file(${ABS_PROJECT_CONFIG_PATH}/${HEADER} ${DEST} COPYONLY)
endforeach(HEADER ${PRJ_CONF_FILES})

# Skip board and app configs to have predictable environment - to be used by CI
if(NOT KISO_STATIC_CONFIG)
    # Copy board-specific config files in intermediary directory
    message(STATUS "Applying board-specific configuration from ${ABS_BOARD_CONFIG_PATH}.")
    file(GLOB_RECURSE BOARD_CONF_FILES
        LIST_DIRECTORIES false
        RELATIVE ${ABS_BOARD_CONFIG_PATH}
        ${ABS_BOARD_CONFIG_PATH}/*.h
    )

    foreach(HEADER ${BOARD_CONF_FILES})
        set(DEST ${KISO_CONFIG_PATH})
        get_filename_component(SUBDIR ${HEADER} DIRECTORY)
        if(SUBDIR)
            set(DEST ${KISO_CONFIG_PATH}/${SUBDIR})
            file(MAKE_DIRECTORY ${DEST})
        endif(SUBDIR)
        configure_file(${ABS_BOARD_CONFIG_PATH}/${HEADER} ${DEST} COPYONLY)
    endforeach(HEADER ${BOARD_CONF_FILES})

    # Copy app-specific config files in intermediary directory
    # APP_CONFIG_PATH is not required - only act if present
    if(NOT APP_CONFIG_PATH)
        message(STATUS "APP_CONFIG_PATH not set to a valid path. Not using application-specific configuration.")
    else()
        get_filename_component(ABS_APP_CONFIG_PATH ${APP_CONFIG_PATH} REALPATH)
        if(NOT EXISTS ${ABS_APP_CONFIG_PATH})
            message(SEND_ERROR "APP_CONFIG_PATH must be absolute or relative to Kiso root!\n"
                            "Current value: ${ABS_APP_CONFIG_PATH}")
        else()
            message(STATUS "Applying application-specific configuration from ${ABS_APP_CONFIG_PATH}.")
            file(GLOB_RECURSE USER_CONF_FILES
                LIST_DIRECTORIES false
                RELATIVE ${ABS_APP_CONFIG_PATH}
                ${ABS_APP_CONFIG_PATH}/*.h
            )

            foreach(HEADER ${USER_CONF_FILES})
                set(DEST ${KISO_CONFIG_PATH})
                get_filename_component(SUBDIR ${HEADER} DIRECTORY)
                if(SUBDIR)
                    set(DEST ${KISO_CONFIG_PATH}/${SUBDIR})
                    file(MAKE_DIRECTORY ${DEST})
                endif(SUBDIR)
                configure_file(${ABS_APP_CONFIG_PATH}/${HEADER} ${DEST} COPYONLY)
            endforeach(HEADER ${USER_CONF_FILES})
        endif(NOT EXISTS ${ABS_APP_CONFIG_PATH})
    endif(NOT APP_CONFIG_PATH)
endif(NOT KISO_STATIC_CONFIG)

message(STATUS "Configuration headers merged in ${KISO_CONFIG_PATH}.")
