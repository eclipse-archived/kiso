cmake_minimum_required(VERSION 3.6)

project ("Kiso Utils" C)

## Interface library
add_library(utils_int INTERFACE)
target_include_directories(utils_int
INTERFACE
   include
)
target_link_libraries(utils_int INTERFACE kiso_config seggerrtt_int)

## Define static library to be compiled
# Make sure we are only compiling with the proper toolchain
if(${CMAKE_CROSSCOMPILING})
   file(GLOB UTILS_SOURCES
      source/*.c
      source/${KISO_TYPE}/*.c
      source/Logging/*.c
      source/Logging/${KISO_OS_LIB}/*.c
   )
   add_library(utils STATIC ${UTILS_SOURCES})
   target_link_libraries(utils utils_int essentials ${KISO_OS_LIB} seggerrtt)
endif(${CMAKE_CROSSCOMPILING})

## Add tests
if(${CMAKE_TESTING_ENABLED})
   add_subdirectory(test)
endif()
