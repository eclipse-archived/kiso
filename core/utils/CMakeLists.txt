cmake_minimum_required(VERSION 3.6)

project ("Kiso Utils" C)

## Interface library
add_library(utils_int INTERFACE)
target_include_directories(utils_int
INTERFACE
   include
   ${KISO_CONFIG_PATH}/utils
)

## Enable static code analysis
#  the checks will be executed as it would be on the desired compile step
if(${ENABLE_STATIC_CHECKS})
   set(CMAKE_C_CLANG_TIDY   ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=${CMAKE_SYSROOT})
   set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=${CMAKE_SYSROOT})
endif()

## Define static library to be compiled
# Make sure we are only compiling with the proper toolchain
if(${CMAKE_CROSSCOMPILING})
   file(GLOB UTILS_SOURCES
      source/*.c
      source/${KISO_OS_LIB}/*.c
      source/Logging/*.c
      source/Logging/${KISO_OS_LIB}/*.c
   )
   add_library(utils STATIC ${UTILS_SOURCES})
   target_link_libraries(utils PUBLIC utils_int essentials ${KISO_OS_LIB})
endif(${CMAKE_CROSSCOMPILING})

## Add tests
if(${CMAKE_TESTING_ENABLED})
   add_subdirectory(test)
endif()
