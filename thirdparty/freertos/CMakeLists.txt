cmake_minimum_required(VERSION 3.6)

project ("FreeRTOS" C)

set(FREERTOS_DIR 3rd-party/FreeRTOS/Source)

# Get the proper port files depending on compiler
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
   set(FREERTOS_COMPILER_STRING "GCC")
   set(PORT_C_LOCATION ${FREERTOS_DIR}/portable/${FREERTOS_COMPILER_STRING}/${KISO_MCU_CORE})
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
   set(FREERTOS_COMPILER_STRING "MSVC-MingW")
   set(PORT_C_LOCATION ${FREERTOS_DIR}/portable/${FREERTOS_COMPILER_STRING})
else()
   message(SEND_ERROR "Unsupported compiler by FreeRTOS: ${CMAKE_C_COMPILER_ID}")
endif()

# Interface library
add_library(freertos_int INTERFACE)
target_include_directories(freertos_int INTERFACE
   ${FREERTOS_DIR}/include
   ${PORT_C_LOCATION}
   ${KISO_CONFIG_PATH}/freertos
)
target_link_libraries(freertos_int INTERFACE essentials_int)

if(${CMAKE_CROSSCOMPILING})
   add_library(freertos STATIC
      source/FreeRTOSConfigCompatibilityCheck.c
      ${FREERTOS_DIR}/croutine.c
      ${FREERTOS_DIR}/event_groups.c
      ${FREERTOS_DIR}/list.c
      ${FREERTOS_DIR}/queue.c
      ${FREERTOS_DIR}/stream_buffer.c
      ${FREERTOS_DIR}/tasks.c
      ${FREERTOS_DIR}/timers.c
      ${FREERTOS_DIR}/portable/MemMang/heap_4.c
      ${PORT_C_LOCATION}/port.c
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
