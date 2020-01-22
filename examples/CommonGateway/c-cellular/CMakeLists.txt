cmake_minimum_required(VERSION 3.6)

project("Kiso Example c-cellular" C)

if (NOT DEFINED APP_APN_NAME)
   message(SEND_ERROR "APP_APN_NAME is a required parameter! Use cmake <...> -DAPP_APN_NAME='\"app-name\"' to specify.")
endif()

if(${CMAKE_CROSSCOMPILING})
   USE_CUSTOM_CONFIG(${CMAKE_CURRENT_LIST_DIR}/config)

   add_executable(application
      source/main.c
      source/App.c
   )
   target_compile_definitions(application PRIVATE -DAPP_APN_NAME="${APP_APN_NAME}")
   target_include_directories(application PRIVATE source)
   target_link_libraries(application bsp essentials utils cellular)

   include(FlashTarget)
   CREATE_FLASH_TARGET_JLINK(application)
endif()
