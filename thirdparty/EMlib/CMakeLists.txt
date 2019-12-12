cmake_minimum_required(VERSION 3.6)

project ("EMlib" C)

set(EMLIB_PATH 3rd-party/EMlib)

if(${CMAKE_CROSSCOMPILING})
   file(GLOB EMLIB_SOURCES
      ${EMLIB_PATH}/emlib/src/*.c
      ${EMLIB_PATH}/Device/SiliconLabs/${KISO_MCU_FAMILY}/source/*.c
   )

   # Defined in board_config.cmake
   if(${EMLIB_INCLUDE_USB})
      file(GLOB EMLIB_USB_SOURCES
         ${EMLIB_PATH}/usb/src/*.c
      )
   endif()

   add_library(emlib STATIC
      ${EMLIB_SOURCES}
      ${EMLIB_USB_SOURCES}
   )
   target_include_directories(emlib
   PUBLIC
      ${EMLIB_PATH}/CMSIS/Include
      ${EMLIB_PATH}/Device/SiliconLabs/${KISO_MCU_FAMILY}/include
      ${EMLIB_PATH}/emlib/inc
   )
   if(${EMLIB_INCLUDE_USB})
      target_include_directories(emlib PUBLIC ${EMLIB_PATH}/usb/inc)
   endif()

   target_link_libraries(emlib kiso_config)
endif()

if(${CMAKE_TESTING_ENABLED})
   # Interface to be used by tests
   add_library(emlib_test_int INTERFACE)
   target_include_directories(emlib_test_int
   INTERFACE
      test/unit/include
      ${EMLIB_PATH}/CMSIS/Include
      ${EMLIB_PATH}/Device/SiliconLabs/${KISO_MCU_FAMILY}/include
      ${EMLIB_PATH}/emlib/inc
   )
   target_link_libraries(emlib_test_int INTERFACE kiso_config)
endif()
