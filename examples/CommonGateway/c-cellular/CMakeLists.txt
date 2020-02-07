cmake_minimum_required(VERSION 3.6)

project("Kiso Example c-cellular" C)

if(${CMAKE_CROSSCOMPILING})
   set(APP_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/config PARENT_SCOPE)

   add_executable(application
      source/main.c
      source/App.c
   )
   target_include_directories(application PRIVATE source)
   target_link_libraries(application bsp essentials utils cellular)

   include(FlashTarget)
   CREATE_FLASH_TARGET_JLINK(application)
endif()
