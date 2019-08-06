cmake_minimum_required(VERSION 3.6)

project ("Kiso Connectivity" C)

## Make sure we are only compiling them with the proper toolchain
if(${CMAKE_CROSSCOMPILING})

   file(GLOB CELLULAR_SOURCES
      cellular/source/common/*.c
      cellular/source/common/ublox/*.c
   )
   add_library(cellular STATIC ${CELLULAR_SOURCES})
   target_include_directories(cellular
   PUBLIC
      cellular/include
   PRIVATE
      cellular/source/protected
      cellular/source/ublox
   )
   target_link_libraries(cellular kiso_config essentials utils ${KISO_OS_LIB} seggerrtt)

endif(${CMAKE_CROSSCOMPILING})

## Add tests
if(${CMAKE_TESTING_ENABLED})
   # TODO
   # add_subdirectory(cellular/test)
endif()
