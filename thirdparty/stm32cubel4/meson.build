### Build the Stm32Cubel4 as library ###

# Get headers
include_dirs = [
	'include',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Inc',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy',
	'./3rd-party/stm32cube/Drivers/CMSIS/Include',
	'./3rd-party/stm32cube/Drivers/CMSIS/Device/ST/STM32L4xx/Include',
	'../../core/essentials/include'] # dependency to be removed in a later point.

# Build include object
module_dirs = include_directories(include_dirs)

# Get common source files
common_files = [
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_can.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_comp.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cryp_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cryp.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dac_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dac.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dfsdm.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_firewall.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hcd.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_irda.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_lcd.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_lptim.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_msp_template.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_nand.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_nor.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_opamp_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_opamp.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_qspi.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sai.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sd.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smartcard_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smartcard.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smbus.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sram.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_swpmi.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tsc.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_usart.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_wwdg.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_fmc.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_sdmmc.c',
	'./3rd-party/stm32cube/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usb.c',
	'source/BCDS_stm32l4xx_hal_dma.c'] # DMA loockup faster then the original version

# Build library
stm32cubel4_lib = static_library('stm32cubel4', 
	common_files, 
	include_directories : [config_dirs, module_dirs], 
	install : true)

# Build dependency object
stm32cubel4_lib_dep = declare_dependency(
	include_directories : [config_dirs, module_dirs], 
	link_with : stm32cubel4_lib)