cmake_minimum_required(VERSION 3.6)

project ("Kiso Example c-leds" C)

# set(APP_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR}/config)
# set(CURRENT_CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR}/config)

# ## Interface for app config - used by almost everything
# add_library(app_config INTERFACE)
# target_include_directories(app_config
# INTERFACE
#    ${CURRENT_CONFIG_DIR}
#    ${CURRENT_CONFIG_DIR}/Cellular
#    ${CURRENT_CONFIG_DIR}/Utils
# )

if(${CMAKE_CROSSCOMPILING})
   add_executable(application
      source/main.c
      source/blinky_led.c
   )
   target_include_directories(application PRIVATE source)
   target_link_libraries(application bsp essentials utils ${KISO_OS_LIB} ${KISO_BOARD_LIBS})

   add_custom_target(application.bin ALL
      COMMAND ${CMAKE_OBJCOPY} -O binary -R .usrpg $<TARGET_FILE:application> ${CMAKE_CURRENT_BINARY_DIR}/application.bin
      COMMENT "Creating flashable binary ${CMAKE_CURRENT_BINARY_DIR}/application.bin"
   )
   add_dependencies(application.bin application)

   include(FlashTarget)
   CREATE_FLASH_TARGET_JLINK(application)
endif()

# Generate/copy app-scope middleware config files from configuration repository
# configure_file(${KISO_CENTRAL_CONFIG_DIR}/Cellular/Kiso_CellularConfig.h ${CURRENT_CONFIG_DIR}/Cellular/Kiso_CellularConfig.h COPYONLY)
# include(${APP_CONFIG_DIR}/Utils/Kiso_UtilsConfig.cmake)
# configure_file(${KISO_CENTRAL_CONFIG_DIR}/Utils/LogConfig.h ${CURRENT_CONFIG_DIR}/Utils/LogConfig.h COPYONLY)
# configure_file(${KISO_CENTRAL_CONFIG_DIR}/Utils/PipeAndFilterConfig.h ${CURRENT_CONFIG_DIR}/Utils/PipeAndFilterConfig.h COPYONLY)
# configure_file(${KISO_CENTRAL_CONFIG_DIR}/Utils/ErrorLoggerConfig.h ${CURRENT_CONFIG_DIR}/Utils/ErrorLoggerConfig.h COPYONLY)
# configure_file(${KISO_CENTRAL_CONFIG_DIR}/Utils/Kiso_EventHubConfig.h ${CURRENT_CONFIG_DIR}/Utils/Kiso_EventHubConfig.h COPYONLY)
# configure_file(${KISO_CENTRAL_CONFIG_DIR}/FreeRTOSConfig.h ${CURRENT_CONFIG_DIR}/FreeRTOSConfig.h COPYONLY)
