cmake_minimum_required(VERSION 3.6)

project ("Kiso Board" C ASM)

## Enable static code analysis
#  the checks will be executed as it would be on the desired compile step
if(${ENABLE_STATIC_CHECKS})
   set(CMAKE_C_CLANG_TIDY   ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi --extra-arg=-mthumb --extra-arg=--sysroot=${CMAKE_SYSROOT} -checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*)
   set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi                     --extra-arg=--sysroot=${CMAKE_SYSROOT} -checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*)
endif()

## Only compilable for target board
if(${CMAKE_CROSSCOMPILING})
   # BSP library for selected board
   file(GLOB BSP_SOURCES
      ${KISO_BOARD_NAME}/bsp/source/*.c
      ${KISO_BOARD_NAME}/bsp/source/*.s
      ${KISO_BOARD_NAME}/bsp/source/*.S
      ${KISO_BOARD_NAME}/bsp/source/protected/*.c
   )
   add_library(bsp STATIC ${BSP_SOURCES})
   target_include_directories(bsp
   PUBLIC
      ${KISO_BOARD_NAME}/bsp/include
   PRIVATE
      ${KISO_BOARD_NAME}/bsp/source/protected
   )
   # List of additional libs from board_config.cmake
   target_link_libraries(bsp PUBLIC essentials ${KISO_BOARD_LIBS})
endif(${CMAKE_CROSSCOMPILING})

# Include the tests for this module
if(${CMAKE_TESTING_ENABLED})
   #add_subdirectory(bsp/test)
endif()
