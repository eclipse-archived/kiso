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

find_program(CLANG_FORMAT clang-format
    NAMES clang-format-10 clang-format-9 clang-format-8 clang-format-7)
message(STATUS "Looking for clang-format: ${CLANG_FORMAT}")

add_custom_target(fix_formatting)

function(ADD_TO_FORMATTING_TARGET DIRS_TO_SCAN FILETYPES_TO_SCAN SKIP_FORMAT_REPORTS)
    set(GLOBBING_EXPRESSIONS "")
    foreach(DIR ${DIRS_TO_SCAN})
        foreach(TYPE ${FILETYPES_TO_SCAN})
            list(APPEND GLOBBING_EXPRESSIONS "${DIR}/${TYPE}")
        endforeach(TYPE ${FILETYPES_TO_SCAN})
    endforeach(DIR ${DIRS_TO_SCAN})

    file(GLOB_RECURSE FILES_TO_FORMAT ${GLOBBING_EXPRESSIONS})

    list(LENGTH FILES_TO_FORMAT NB_FILES_TO_FORMAT)
    if(${SKIP_FORMAT_REPORTS})
        message(STATUS "Generating format target for ${NB_FILES_TO_FORMAT} files.")
    else()
        message(STATUS "Generating format reports and format target for ${NB_FILES_TO_FORMAT} files. This may take some time...")
    endif()
    foreach(SRC_FILE ${FILES_TO_FORMAT})
        if(NOT ${SKIP_FORMAT_REPORTS})
            file(RELATIVE_PATH REL_SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_FILE})
            get_filename_component(REL_SRC_DIR ${REL_SRC_FILE} DIRECTORY)
            get_filename_component(FILE ${REL_SRC_FILE} NAME)
            string(CONCAT XML_FILE "${CMAKE_CURRENT_BINARY_DIR}/" "${REL_SRC_DIR}/" "${FILE}_format.xml")
            get_filename_component(XML_DIR ${XML_FILE} DIRECTORY)
            file(MAKE_DIRECTORY ${XML_DIR})
            execute_process(COMMAND ${CLANG_FORMAT} -style=file -output-replacements-xml ${SRC_FILE} OUTPUT_FILE ${XML_FILE})
        endif()
        add_custom_command(TARGET fix_formatting
            COMMAND ${CLANG_FORMAT} -style=file -i ${SRC_FILE})
    endforeach()
endfunction(ADD_TO_FORMATTING_TARGET DIRS_TO_SCAN FILETYPES_TO_SCAN)
