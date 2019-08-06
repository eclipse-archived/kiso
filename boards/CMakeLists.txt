cmake_minimum_required(VERSION 3.6)

project ("Kiso Board" C ASM)

## Interface for kiso config - used by almost everything
add_library(kiso_config INTERFACE)
target_include_directories(kiso_config
INTERFACE
   ${KISO_BOARD_NAME}/kiso_config
   ${KISO_BOARD_NAME}/kiso_config/Essentials
   ${KISO_BOARD_NAME}/kiso_config/Utils
   ${KISO_BOARD_NAME}/kiso_config/SeggerRTT
   ${KISO_BOARD_NAME}/kiso_config/Cellular
)

## Only compilable for target board
if(${CMAKE_CROSSCOMPILING})
   # BSP library for selected board
   file(GLOB BSP_SOURCES
      ${KISO_BOARD_NAME}/bsp/source/*.c
      ${KISO_BOARD_NAME}/bsp/source/*.s
      ${KISO_BOARD_NAME}/bsp/source/*.S
   )
   add_library(bsp STATIC ${BSP_SOURCES})
   target_include_directories(bsp
   PUBLIC
      ${KISO_BOARD_NAME}/bsp/include
   PRIVATE
      ${KISO_BOARD_NAME}/bsp/source/protected
   )
   # List of additional libs from board_config.cmake
   target_link_libraries(bsp essentials ${KISO_BOARD_LIBS})
endif(${CMAKE_CROSSCOMPILING})

# Include the tests for this module
if(${CMAKE_TESTING_ENABLED})
   #add_subdirectory(bsp/test)
endif()
