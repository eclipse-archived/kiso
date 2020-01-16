cmake_minimum_required(VERSION 3.6)

project ("Kiso Example c-leds" C)

if(${CMAKE_CROSSCOMPILING})
   add_executable(application
      source/main.c
      source/blinky_led.c
   )
   target_include_directories(application PRIVATE source)
   target_link_libraries(application bsp essentials utils)

   include(FlashTarget)
   CREATE_FLASH_TARGET_JLINK(application)
endif()
