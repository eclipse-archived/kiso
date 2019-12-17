cmake_minimum_required(VERSION 3.6)

project ("Kiso Example c-sensors" C)

set(KISO_APP_LIBS bstlib PARENT_SCOPE)

if(${CMAKE_CROSSCOMPILING})
   USE_CUSTOM_CONFIG(${CMAKE_CURRENT_LIST_DIR}/config)

   add_executable(application
      source/main.c
      source/sensors_common.c
      source/env_sensor.c
      source/accel_sensor.c
   )
   target_include_directories(application PRIVATE source)
   target_link_libraries(application bsp essentials utils bstlib)

   include(FlashTarget)
   CREATE_FLASHABLE_BINARY(application)
   CREATE_FLASH_TARGET_JLINK(application)
endif()
