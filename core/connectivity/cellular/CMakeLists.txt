cmake_minimum_required(VERSION 3.6)

project ("Kiso Cellular" C)

## Interface library
add_library(cellular_int INTERFACE)
target_include_directories(cellular_int
INTERFACE
   include
   ${KISO_CONFIG_PATH}/cellular
)
target_link_libraries(cellular_int INTERFACE essentials_int)

## Enable static code analysis
#  the checks will be executed as it would be on the desired compile step
if(${ENABLE_STATIC_CHECKS})
   set(CMAKE_C_CLANG_TIDY   ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=${CMAKE_SYSROOT})
   set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=${CMAKE_SYSROOT})
endif()

## Make sure we are only compiling them with the proper toolchain
if(${CMAKE_CROSSCOMPILING})

   file(GLOB CELLULAR_SOURCES
      source/common/*.c
      source/ublox/*.c
   )
   add_library(cellular STATIC EXCLUDE_FROM_ALL ${CELLULAR_SOURCES})
   target_include_directories(cellular
   PUBLIC
      include
   PRIVATE
      source/protected
      source/ublox
   )
   target_link_libraries(cellular cellular_int essentials utils ${KISO_OS_LIB})

endif(${CMAKE_CROSSCOMPILING})

## Add tests
if(${CMAKE_TESTING_ENABLED})
   add_subdirectory(test)
endif()
