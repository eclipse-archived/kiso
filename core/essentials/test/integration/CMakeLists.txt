cmake_minimum_required(VERSION 3.6)

project ("Essentials integration test entry" C ASM)

#  the checks will be executed as it would be on the desired compile step
if(${ENABLE_STATIC_CHECKS})
   set(CMAKE_C_CLANG_TIDY   ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi --extra-arg=-mthumb --extra-arg=--sysroot=${CMAKE_SYSROOT} -checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*)
   set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi                     --extra-arg=--sysroot=${CMAKE_SYSROOT} -checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*)
endif()

## Only compilable for a target
if(${CMAKE_CROSSCOMPILING})
   file(GLOB TEST_ENTRY_SOURCES
      source/*.c
   )
   add_library(testentry STATIC ${TEST_ENTRY_SOURCES})
   
   target_include_directories(testentry
      PRIVATE
      source
   )
   # List of additional libs from board_config.cmake
   target_link_libraries(testentry testing essentials ${KISO_BOARD_LIBS})
endif(${CMAKE_CROSSCOMPILING})

# Include the tests for this module
if(${CMAKE_TESTING_ENABLED})
   #add_subdirectory(testentry/test)
endif()

