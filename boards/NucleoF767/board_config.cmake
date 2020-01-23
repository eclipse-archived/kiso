## Define here all the build options specific for the board and compiler toolchain,
## along with definitions for the libraries

## Kiso configuration
# MCU configuration for this board
set(KISO_MCU_FAMILY stm32)
set(KISO_MCU_SERIES stm32f7)
set(KISO_MCU_TYPE   STM32F767xx)
set(KISO_MCU_DEVICE STM32F767ZITx)
# set(KISO_MCU_REVISION B01)

# GCC portmacro to use in freertos
set(KISO_MCU_CORE_VAR r0p1)
set(KISO_MCU_CORE "ARM_CM7/${KISO_MCU_CORE_VAR}")

if(${ENABLE_STATIC_CHECKS})
   set(KISO_MCU_CORE ARM_CM3)
endif()

## Libs configuration
# Additional thirdparty libraries for the board
set(KISO_BOARD_LIBS stm32cubef7)

## Board definitions
# Compile definitions for all subprojects after this call
add_definitions(
   -DKISO_PACKAGE_ID=0
   -D${KISO_MCU_FAMILY}
   -D${KISO_MCU_SERIES}
   -D${KISO_MCU_TYPE}
   -D${KISO_MCU_DEVICE}
   -DAPPLICATION_ENTRY_ADDRESS=0x08000000
   -DAPPLICATION_SECTION_SIZE=0x80000
)

if(${CMAKE_CROSSCOMPILING} AND "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
   ## Compiler and linker configuration
   # GCC options common to compiler and linker
   if(${ENABLE_STATIC_CHECKS})
      set(BOARD_COMMON_OPTIONS
      -march=armv7e-m
      -mtune=cortex-m3
      -mfloat-abi=soft
      -mfpu=fpv4-sp-d16
      -mthumb
      )
   else()
      set(BOARD_COMMON_OPTIONS
         -march=armv7e-m
         -mtune=cortex-m7
         -mfloat-abi=softfp
         -mfpu=fpv4-sp-d16
         -mthumb
         -mno-thumb-interwork
         )
   endif()

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
      -T${CMAKE_CURRENT_LIST_DIR}/Application_STM32F7xx_FLASH.ld
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
   # Join the items on the list separated by a space and write the result in CMAKE_EXE_LINKER_FLAGS
   string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS "${BOARD_LINKER_FLAGS}")

endif()