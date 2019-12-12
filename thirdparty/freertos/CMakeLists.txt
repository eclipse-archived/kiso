cmake_minimum_required(VERSION 3.6)

project ("FreeRTOS" C)

set(FREERTOS_PATH 3rd-party/FreeRTOS/Source)

# Get the proper port files depending on compiler
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
   set(FREERTOS_COMPILER_STRING "GCC")
   set(FREERTOS_PORT_PATH ${FREERTOS_PATH}/portable/${FREERTOS_COMPILER_STRING}/${KISO_MCU_CORE})
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
   set(FREERTOS_COMPILER_STRING "MSVC-MingW")
   set(FREERTOS_PORT_PATH ${FREERTOS_PATH}/portable/${FREERTOS_COMPILER_STRING})
else()
   message(SEND_ERROR "Unsupported compiler by FreeRTOS: ${CMAKE_C_COMPILER_ID}")
endif()

# Interface library
add_library(freertos_int INTERFACE)
target_include_directories(freertos_int INTERFACE
   ${FREERTOS_PATH}/include
   ${FREERTOS_PORT_PATH}
   ${KISO_CONFIG_PATH}/thirdparty/freertos
)
target_link_libraries(freertos_int INTERFACE essentials_int)

if(${CMAKE_CROSSCOMPILING})
   add_library(freertos STATIC
      source/FreeRTOSConfigCompatibilityCheck.c
      ${FREERTOS_PATH}/croutine.c
      ${FREERTOS_PATH}/event_groups.c
      ${FREERTOS_PATH}/list.c
      ${FREERTOS_PATH}/queue.c
      ${FREERTOS_PATH}/stream_buffer.c
      ${FREERTOS_PATH}/tasks.c
      ${FREERTOS_PATH}/timers.c
      ${FREERTOS_PATH}/portable/MemMang/heap_4.c
      ${FREERTOS_PORT_PATH}/port.c
   )
   target_link_libraries(freertos freertos_int)
endif()

if(${CMAKE_TESTING_ENABLED})
   # Interface to be used by tests
   add_library(freertos_test_int INTERFACE)
   target_include_directories(freertos_test_int INTERFACE
      test/unit/include
   )
   target_link_libraries(freertos_test_int INTERFACE freertos_int)
endif()
