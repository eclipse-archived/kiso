cmake_minimum_required(VERSION 3.6)

project ("STM32CubeL4" C ASM)

set(STM32CUBEL4_DIR 3rd-party/stm32cube)

# Make sure we are only compiling with the proper toolchain
if(${CMAKE_CROSSCOMPILING})
   add_library(stm32cubel4 STATIC
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_can.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_comp.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cryp_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cryp.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dac_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dac.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dfsdm.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_firewall.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hcd.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_irda.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_lcd.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_lptim.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_nand.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_nor.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_opamp_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_opamp.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_qspi.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sai.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sd.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smartcard_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smartcard.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smbus.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sram.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_swpmi.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tsc.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_usart.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_wwdg.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_fmc.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_sdmmc.c
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usb.c
   )
   # Newer arm-gcc versions have these enabled by default
   include(CheckCCompilerFlag)
   check_c_compiler_flag(-Wno-error=tautological-compare has_no_tautological_compare)
   if(${has_no_tautological_compare})
      target_compile_options(stm32cubel4 PRIVATE -Wno-error=tautological-compare)
   endif()
   check_c_compiler_flag(-Wno-error=attributes has_no_attributes)
   if(${has_no_attributes})
      target_compile_options(stm32cubel4 PRIVATE -Wno-error=attributes)
   endif()
   target_compile_options(stm32cubel4 PRIVATE
      -Wno-unused-parameter
      -Wno-int-to-pointer-cast
   )
   target_include_directories(stm32cubel4
   PUBLIC
      include
      ${STM32CUBEL4_DIR}/Drivers/CMSIS/Include
      ${STM32CUBEL4_DIR}/Drivers/CMSIS/Device/ST/STM32L4xx/Include
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy
      ${KISO_CONFIG_PATH}/stm32cubel4
   )
   target_link_libraries(stm32cubel4 essentials_int)
endif(${CMAKE_CROSSCOMPILING})

if(${CMAKE_TESTING_ENABLED})
   # Define interface library to be used by the tests without compiling the actual library
   add_library(stm32cubel4_test_int INTERFACE)
   target_include_directories(stm32cubel4_test_int INTERFACE
      include
      ${STM32CUBEL4_DIR}/Drivers/CMSIS/Include
      ${STM32CUBEL4_DIR}/Drivers/CMSIS/Device/ST/STM32L4xx/Include
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc
      ${STM32CUBEL4_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy
      test/unit/include
      ${KISO_CONFIG_PATH}/stm32cubel4
   )
   target_compile_options(stm32cubel4_test_int INTERFACE
      -Wno-int-to-pointer-cast
   )
endif(${CMAKE_TESTING_ENABLED})
