cmake_minimum_required(VERSION 3.6)

project ("Kiso Essentials" C)

## Interface library
add_library(essentials_int INTERFACE)
target_include_directories(essentials_int
INTERFACE
   include
   include/bsp
   include/mcu
   include/mcu/${KISO_MCU_FAMILY}
   include/mcu/${KISO_MCU_FAMILY}/${KISO_MCU_SERIES}
)
target_link_libraries(essentials_int INTERFACE kiso_config)


## Enable static code analysis
#  the checks will be executed as it would be on the desired compile step
if(${ENABLE_STATIC_CHECKS})
      set(CMAKE_C_CLANG_TIDY clang-tidy --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=/usr/lib/arm-none-eabi   -checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*)
      set(CMAKE_CXX_CLANG_TIDY clang-tidy --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=/usr/lib/arm-none-eabi -checks=-*,readability-*,clang-analyzer-*,-clang-analyzer-cplusplus*)
endif()

## Define static library to be compiled
# Make sure we are only compiling them with the proper toolchain
if(${CMAKE_CROSSCOMPILING})
   file(GLOB ESSENTIALS_SOURCES
      source/*.c
      source/mcu/${KISO_MCU_FAMILY}/*.c
      source/mcu/${KISO_MCU_FAMILY}/${KISO_MCU_SERIES}/*.c
   )
   add_library(essentials STATIC ${ESSENTIALS_SOURCES})
   target_link_libraries(essentials essentials_int bsp ${KISO_BOARD_LIBS})
endif(${CMAKE_CROSSCOMPILING})


## Add tests
if(${CMAKE_TESTING_ENABLED})
   add_subdirectory(test)
endif()
