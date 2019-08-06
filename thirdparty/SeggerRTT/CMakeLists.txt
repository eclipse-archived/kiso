cmake_minimum_required(VERSION 3.6)

project (SeggerRTT C)

set(SEGGERRTT_DIR 3rd-party/SEGGER_RTT)

add_library(seggerrtt_int INTERFACE)
target_include_directories(seggerrtt_int INTERFACE ${SEGGERRTT_DIR}/RTT)
target_link_libraries(seggerrtt_int INTERFACE kiso_config)

# Make sure we are only compiling with the proper toolchain
if(${CMAKE_CROSSCOMPILING})
   add_library(seggerrtt STATIC
      ${SEGGERRTT_DIR}/RTT/SEGGER_RTT.c
      ${SEGGERRTT_DIR}/RTT/SEGGER_RTT_printf.c
   )
   target_link_libraries(seggerrtt seggerrtt_int ${KISO_OS_LIB} kiso_config essentials_int)
endif()

if(${CMAKE_TESTING_ENABLED})
   # Define interface library to be used by the tests without compiling the actual library
   add_library(seggerrtt_test_int INTERFACE)
   target_include_directories(seggerrtt_test_int INTERFACE test/unit/include)
   target_link_libraries(seggerrtt_test_int INTERFACE seggerrtt_int)
endif()
