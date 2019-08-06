## Define here all the build options specific for the board and compiler toolchain,
## along with definitions for the libraries

## Kiso configuration
# MCU configuration for this board
set(KISO_MCU_TYPE stm32)
set(KISO_MCU_FAMILY stm32l4)
# Additional thirdparty libraries for the board
set(KISO_BOARD_LIBS stm32cubel4)

## Libs configuration
# GCC portmacros to use in freertos
set(FREERTOS_CPU_VARIANCE ARM_CM4F)

## Board definitions
# Compile definitions for all subprojects after this call
add_definitions(
   -DBCDS_PACKAGE_ID=0
   -D${KISO_MCU_TYPE}
   -D${KISO_MCU_FAMILY}
   -DSTM32L486xx
   -DSTM32L486VG
   -DAPPLICATION_ENTRY_ADDRESS=0x08000000
   -DAPPLICATION_SECTION_SIZE=0x80000
)

if(${CMAKE_CROSSCOMPILING} AND "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
   ## Compiler and linker configuration
   # GCC options common to compiler and linker
   set(BOARD_COMMON_OPTIONS
      -march=armv7e-m
      -mtune=cortex-m4
      -mfloat-abi=softfp
      -mfpu=fpv4-sp-d16
      -mthumb
      -mno-thumb-interwork
   )

   # Enable some warnings, debug symbols and set optimization level
   add_compile_options(
      ${BOARD_COMMON_OPTIONS}
      -ffunction-sections
      -fdata-sections
      -Wall
      -Wextra
      -Wmissing-prototypes
      -Wstrict-prototypes
      -O0
      -g
   )

   # Use board-specific linkerfile and put system libs in their own section
   # CMake 3.13 and up has add_link_options which does the same as this hack
   set(BOARD_LINKER_FLAGS
      ${BOARD_COMMON_OPTIONS}
      --specs=nosys.specs
      -T${CMAKE_CURRENT_LIST_DIR}/Application_STM32L4xx_FLASH.ld
      -Xlinker
      -Map=out.map
      -Wl,--gc-sections
      -Wl,--start-group
      -lgcc
      -lc
      -lnosys
      -lm
      -Wl,--end-group
      -Wl,-u,_printf_float
   )
   # Join the items on the list with a space between them and write the result in CMAKE_EXE_LINKER_FLAGS
   string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS "${BOARD_LINKER_FLAGS}")

elseif(${CMAKE_TESTING_ENABLED})
   # Add some warnings and debug symbols in unit tests
   add_compile_options(-g -Wall -Wextra)
endif()
