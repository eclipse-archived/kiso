cmake_minimum_required(VERSION 3.6)

project ("lwip" C)

set(LWIP_DIR 3rd-party/lwip)
set(LWIP_OSAL_DIR 3rd-party/lwip_osal)
set(LWIP_INCLUDE_DIRS
	${LWIP_DIR}/src/include
	${LWIP_DIR}/src/include)

set(CURRENT_CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR}/config)

include(3rd-party/lwip/src/Filelists.cmake)

if(${CMAKE_CROSSCOMPILING})

add_library(lwip EXCLUDE_FROM_ALL ${lwipnoapps_SRCS})
target_compile_options(lwip PRIVATE
      -Wno-implicit-function-declaration
      -Wno-unused-value
      -Wno-strict-overflow
      -Wno-address
      -Wno-missing-prototypes
)
target_include_directories(lwip PRIVATE
	${CURRENT_CONFIG_DIR}
	${LWIP_INCLUDE_DIRS}
	${LWIP_OSAL_DIR}/include
)
target_link_libraries(lwip freertos_int)

configure_file(${KISO_CENTRAL_CONFIG_DIR}/FreeRTOSConfig.h ${CURRENT_CONFIG_DIR}/FreeRTOSConfig.h COPYONLY)
configure_file(${KISO_CENTRAL_CONFIG_DIR}/lwipopts.h ${CURRENT_CONFIG_DIR}/lwipopts.h COPYONLY)

endif(${CMAKE_CROSSCOMPILING})
