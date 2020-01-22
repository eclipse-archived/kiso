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

if(NOT PROJECT_CONFIG_PATH OR NOT EXISTS ${PROJECT_CONFIG_PATH})
    message(SEND_ERROR "PROJECT_CONFIG_PATH must be set to a valid path, containing configuration headers!")
endif()

# This variable is internal and managed through this file only
set(KISO_CONFIG_PATH ${PROJECT_CONFIG_PATH} CACHE PATH "Kiso config headers location")

## Create Kiso config header directory for build
# USER_DIRECTORY is a directory containing user-provided configuration headers for
# Kiso libraries in the same structure as the root config directory
# Missing required headers will be copied over from root config directory
# /todo: Currently supports only one subdir level - allow recursion
function(USE_CUSTOM_CONFIG USER_DIRECTORY)
    if(NOT USER_DIRECTORY OR NOT EXISTS ${USER_DIRECTORY})
        message(SEND_ERROR "User config directory not found: ${USER_DIRECTORY}")
    endif()
    message(STATUS "Using custom application configuration from ${USER_DIRECTORY}")

    set(INT_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR}/config)

    # List all files in application config directory and add them to final config directory
    file(GLOB_RECURSE USER_CONF_FILES
        LIST_DIRECTORIES false
        RELATIVE ${USER_DIRECTORY}
        ${USER_DIRECTORY}/*.h
    )

    foreach(HEADER ${USER_CONF_FILES})
        set(DEST ${INT_CONFIG_PATH})
        get_filename_component(SUBDIR ${HEADER} DIRECTORY)
        if(SUBDIR)
            set(DEST ${INT_CONFIG_PATH}/${SUBDIR})
            file(MAKE_DIRECTORY ${DEST})
        endif(SUBDIR)
        configure_file(${USER_DIRECTORY}/${HEADER} ${DEST} COPYONLY)
    endforeach(HEADER ${USER_CONF_FILES})

    # List all files in Kiso config directory and add them to final config directory
    # in case the user hasn't provided one already
    file(GLOB_RECURSE ALL_CONF_FILES
        LIST_DIRECTORIES false
        RELATIVE ${PROJECT_CONFIG_PATH}
        ${PROJECT_CONFIG_PATH}/*.h
    )

    foreach(HEADER ${ALL_CONF_FILES})
        if(NOT EXISTS ${INT_CONFIG_PATH}/${HEADER})
            set(DEST ${INT_CONFIG_PATH})
            get_filename_component(SUBDIR ${HEADER} DIRECTORY)
            if(SUBDIR)
                set(DEST ${INT_CONFIG_PATH}/${SUBDIR})
                file(MAKE_DIRECTORY ${DEST})
            endif(SUBDIR)
            configure_file(${PROJECT_CONFIG_PATH}/${HEADER} ${DEST} COPYONLY)
        endif()
    endforeach(HEADER ${USER_CONF_FILES})

    # Enforce usage of the combined headers directory
    set(KISO_CONFIG_PATH ${INT_CONFIG_PATH} CACHE PATH "Kiso config headers location" FORCE)

    message(STATUS "Intermediate config path: ${KISO_CONFIG_PATH}")
endfunction(USE_CUSTOM_CONFIG USER_DIRECTORY)
