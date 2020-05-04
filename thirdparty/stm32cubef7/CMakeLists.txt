cmake_minimum_required(VERSION 3.6)

project ("STM32CubeF7" C ASM)

set(STM32CUBEF7_DIR 3rd-party/stm32cube)

# Make sure we are only compiling with the proper toolchain
if(${CMAKE_CROSSCOMPILING})
   add_library(stm32cubef7 STATIC
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_can.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cec.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cryp_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cryp.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dac_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dac.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dfsdm.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_eth.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_hash_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_hash.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_hcd.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2s.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_irda.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_iwdg.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_jpeg.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_lptim.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_mdios.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_mmc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_nand.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_nor.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pcd_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pcd.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rng.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_smartcard_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_smartcard.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_smbus.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spdifrx.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sram.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart_ex.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_usart.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_wwdg.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_adc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_crc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dac.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma2d.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_exti.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_gpio.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_i2c.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_lptim.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_pwr.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rcc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rng.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rtc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_spi.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_tim.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usart.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usb.c
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_utils.c
   )
   # Newer arm-gcc versions have these enabled by default
   include(CheckCCompilerFlag)
   check_c_compiler_flag(-Wno-error=tautological-compare has_no_tautological_compare)
   if(${has_no_tautological_compare})
      target_compile_options(stm32cubef7 PRIVATE -Wno-error=tautological-compare)
   endif()
   check_c_compiler_flag(-Wno-error=attributes has_no_attributes)
   if(${has_no_attributes})
      target_compile_options(stm32cubef7 PRIVATE -Wno-error=attributes)
   endif()
   target_compile_options(stm32cubef7 PRIVATE
      -Wno-unused-parameter
      -Wno-int-to-pointer-cast
      -Wno-unused-const-variable
   )
   target_include_directories(stm32cubef7
   PUBLIC
      include
      ${STM32CUBEF7_DIR}/Drivers/CMSIS/Include
      ${STM32CUBEF7_DIR}/Drivers/CMSIS/Device/ST/STM32F7xx/Include
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
      ${KISO_CONFIG_PATH}/thirdparty/stm32cubef7
   )
   target_link_libraries(stm32cubef7 essentials_int)
endif(${CMAKE_CROSSCOMPILING})

if(${CMAKE_TESTING_ENABLED})
   # Define interface library to be used by the tests without compiling the actual library
   add_library(stm32cubef7_test_int INTERFACE)
   target_include_directories(stm32cubef7_test_int INTERFACE
      include
      ${STM32CUBEF7_DIR}/Drivers/CMSIS/Include
      ${STM32CUBEF7_DIR}/Drivers/CMSIS/Device/ST/STM32F7xx/Include
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc
      ${STM32CUBEF7_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
      test/unit/include
   )
   target_compile_options(stm32cubef7_test_int INTERFACE
      -Wno-int-to-pointer-cast
   )
endif(${CMAKE_TESTING_ENABLED})
