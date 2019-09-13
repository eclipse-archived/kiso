set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# The toolchain is supposed to be in PATH
# Or ARM_TOOLCHAIN_DIR should be specified explicitly

if(NOT ARM_TOOLCHAIN_PREFIX)
    set(ARM_TOOLCHAIN_PREFIX arm-none-eabi-)
endif()

if(NOT ARM_TOOLCHAIN_DIR)
    if(CMAKE_HOST_WIN32)
        set(WHICH_CMD where)
    else()
        set(WHICH_CMD which)
    endif()
    execute_process(
        COMMAND ${WHICH_CMD} ${ARM_TOOLCHAIN_PREFIX}gcc
        OUTPUT_VARIABLE FOUND_GCC_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(CMAKE_HOST_WIN32)
        # 'where' on windows returns all found - choose the first one in case there are multiples
        string(FIND ${FOUND_GCC_PATH} "\n" NLPOS)
        if(NOT ${NLPOS} EQUAL -1)
            string(SUBSTRING ${FOUND_GCC_PATH} 0 ${NLPOS} FOUND_GCC_PATH)
        endif()
    endif()
    get_filename_component(ARM_TOOLCHAIN_DIR ${FOUND_GCC_PATH} DIRECTORY)
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(CMAKE_HOST_WIN32)
    set(TOOL_EXT ".exe")
else()
    set(TOOL_EXT "")
endif()

set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN_DIR}/${ARM_TOOLCHAIN_PREFIX}gcc${TOOL_EXT} CACHE INTERNAL "c compiler")
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER} CACHE INTERNAL "asm compiler")
set(CMAKE_CXX_COMPILER ${ARM_TOOLCHAIN_DIR}/${ARM_TOOLCHAIN_PREFIX}g++${TOOL_EXT} CACHE INTERNAL "cxx compiler")

set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${ARM_TOOLCHAIN_PREFIX}objcopy${TOOL_EXT} CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${ARM_TOOLCHAIN_PREFIX}size${TOOL_EXT} CACHE INTERNAL "size tool")

# set(CMAKE_SYSROOT /usr/lib/arm-none-eabi)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
