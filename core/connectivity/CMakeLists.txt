cmake_minimum_required(VERSION 3.6)

project ("Kiso Connectivity" C)

## Interface library
add_library(cellular_int INTERFACE)
target_include_directories(cellular_int
INTERFACE
   cellular/include
)
target_link_libraries(cellular_int INTERFACE essentials_int)

## Make sure we are only compiling them with the proper toolchain
if(${CMAKE_CROSSCOMPILING})

   file(GLOB CELLULAR_SOURCES
      cellular/source/common/*.c
      cellular/source/common/ublox/*.c
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
