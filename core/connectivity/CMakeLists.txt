cmake_minimum_required(VERSION 3.6)

project ("Kiso Connectivity" C)

## Interface library
add_library(cellular_int INTERFACE)
target_include_directories(cellular_int
INTERFACE
   cellular/include
)
target_link_libraries(cellular_int INTERFACE essentials_int)

## Enable static code analysis
#  the checks will be executed as it would be on the desired compile step
if(${ENABLE_STATIC_CHECKS})
      set(CMAKE_C_CLANG_TIDY clang-tidy --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=/usr/lib/arm-none-eabi   -checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*)
      set(CMAKE_CXX_CLANG_TIDY clang-tidy --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=/usr/lib/arm-none-eabi -checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*)
endif()
# TODO: Reenable warnings when module and tests are fixed
add_compile_options(-Wno-cast-qual)

## Make sure we are only compiling them with the proper toolchain
if(${CMAKE_CROSSCOMPILING})

   file(GLOB CELLULAR_SOURCES
      cellular/source/common/*.c
      cellular/source/ublox/*.c
   )
   add_library(cellular STATIC EXCLUDE_FROM_ALL ${CELLULAR_SOURCES})
   target_include_directories(cellular
   PUBLIC
      cellular/include
   PRIVATE
      cellular/source/protected
      cellular/source/ublox
   )
   target_link_libraries(cellular cellular_int kiso_config essentials utils ${KISO_OS_LIB} seggerrtt)

endif(${CMAKE_CROSSCOMPILING})

## Add tests
if(${CMAKE_TESTING_ENABLED})
   add_subdirectory(cellular/test)
endif()
