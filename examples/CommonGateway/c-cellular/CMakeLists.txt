cmake_minimum_required(VERSION 3.6)

project("Kiso Example c-cellular" C)

if (NOT DEFINED APP_APN_NAME)
   message(WARNING "APP_APN_NAME is a required parameter! Use cmake <...> -DAPP_APN_NAME=... to specify.")
endif()

if(${CMAKE_CROSSCOMPILING})
   add_executable(application
      source/main.c
      source/App.c
   )
   target_include_directories(application PRIVATE source generated)
   target_link_libraries(application bsp essentials utils cellular ${KISO_OS_LIB} ${KISO_BOARD_LIBS})

   add_custom_target(application.bin ALL
      COMMAND ${CMAKE_OBJCOPY} -O binary -R .usrpg application application.bin
      COMMENT "Creating flashable binary ${CMAKE_CURRENT_LIST_DIR}/application.bin"
   )
   add_dependencies(application.bin application)
endif()

# App Confiuration
set(APP_CMDPROCESSOR_MAIN_PRIORITY 1 CACHE STRING "Main CmdProcessor task priority")
set(APP_CMDPROCESSOR_MAIN_STACKSIZE 1024 CACHE STRING "Main CmdProcessor stack-size")
set(APP_CMDPROCESSOR_MAIN_QLEN 10 CACHE STRING "Main CmdProcessor queue length")

set(APP_TASK_APP_PRIORITY 2 CACHE STRING "App task priority")
set(APP_TASK_APP_STACKSIZE 1024 CACHE STRING "App stack-size")

set(APP_NUMBER_OF_RETRIES 3 CACHE STRING "App number of retries")
set(APP_SIM_PIN NULL CACHE STRING "App SIM PIN")
set(APP_RAT CELLULAR_RAT_LTE_CAT_M1 CACHE STRING "App Radio Access Technology")
set(APP_APN_NAME ${APP_APN_NAME} CACHE STRING "App APN name")
set(APP_DNS_TEST_FQDN echo.u-blox.com CACHE STRING "App DNS test FQDN (currently not supported)")
set(APP_UDP_TEST_FQDN dummy CACHE STRING "App UDP test FQDN (currently not supported)")
unset(APP_UDP_TEST_FQDN CACHE)
set(APP_UDP_TEST_IP "(Cellular_IpAddress_T){.Type = CELLULAR_IPADDRESSTYPE_IPV4, .Address.IPv4[3] = 195, .Address.IPv4[2] = 34, .Address.IPv4[1] = 89, .Address.IPv4[0] = 241}" CACHE STRING "App UDP test IP")
set(APP_UDP_TEST_PORT 7 CACHE STRING "App UDP test port")
set(APP_UDP_TEST_DATA "HELLO WORLD!" CACHE STRING "App UDP test data")
set(APP_TCP_TEST_FQDN dummy CACHE STRING "App TCP test FQDN (currently not supported)")
unset(APP_TCP_TEST_FQDN CACHE)
set(APP_TCP_TEST_IP "(Cellular_IpAddress_T){.Type = CELLULAR_IPADDRESSTYPE_IPV4, .Address.IPv4[3] = 195, .Address.IPv4[2] = 34, .Address.IPv4[1] = 89, .Address.IPv4[0] = 241}" CACHE STRING "App TCP test IP")
set(APP_TCP_TEST_PORT 7 CACHE STRING "App TCP test port")
set(APP_TCP_TEST_DATA "HELLO WORLD!" CACHE STRING "App TCP test data")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/AppConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/generated/AppConfig_gen.h)
