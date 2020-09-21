cmake_minimum_required(VERSION 3.6)

project ("Kiso Testing" C)

## Interface library
add_library(testing_int INTERFACE)
target_include_directories(testing_int
INTERFACE
   include
)
target_link_libraries(testing_int INTERFACE essentials_int)

## Enable static code analysis
#  the checks will be executed as it would be on the desired compile step
if(${ENABLE_STATIC_CHECKS})
   set(CMAKE_C_CLANG_TIDY   ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=${CMAKE_SYSROOT})
   set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} --extra-arg=--target=arm-none-eabi --extra-arg=--sysroot=${CMAKE_SYSROOT})
endif()

## Make sure we are only compiling them with the proper toolchain
if(${CMAKE_CROSSCOMPILING})

   file(GLOB TESTING_SOURCES
      ./source/*.c
   )
   add_library(testing STATIC EXCLUDE_FROM_ALL ${TESTING_SOURCES})
   target_include_directories(testing
   PUBLIC
      include
   PRIVATE
      source/protected
   )
   target_link_libraries(testing testing_int essentials utils ${KISO_OS_LIB})

endif(${CMAKE_CROSSCOMPILING})

## Add tests
if(${CMAKE_TESTING_ENABLED})
   #add_subdirectory(test)
endif()
