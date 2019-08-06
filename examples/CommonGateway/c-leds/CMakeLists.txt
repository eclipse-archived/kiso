cmake_minimum_required(VERSION 3.6)

project ("Kiso Example c-leds" C)

if(${CMAKE_CROSSCOMPILING})
   add_executable(application
      source/main.c
      source/blinky_led.c
   )
   target_include_directories(application PRIVATE source)
   target_link_libraries(application bsp essentials utils ${KISO_OS_LIB} ${KISO_BOARD_LIBS})

   add_custom_target(application.bin ALL
      COMMAND ${CMAKE_OBJCOPY} -O binary -R .usrpg application application.bin
      COMMENT "Creating flashable binary ${CMAKE_CURRENT_LIST_DIR}/application.bin"
   )
   add_dependencies(application.bin application)
endif()