#Paths to CDDK shared packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials

#Root paths to library packages on which this package depends
BCDS_STM32CUBE_PATH ?= $(THIRD_PARTY_SHARED_PATH)/STM32Cubel4

# additionnal macros needed for compilation
BCDS_MACROS_DEBUG += \
        -D $(BCDS_STM32_DEVICE_PACKAGE_TYPE) \
        -D USE_HAL_DRIVER=1 \
        -D APPLICATION_ENTRY_ADDRESS=$(APPLICATION_ENTRY_ADDRESS) 
BCDS_MACROS_RELEASE += \
        -D $(BCDS_STM32_DEVICE_PACKAGE_TYPE) \
        -D USE_HAL_DRIVER=1 \
        -D APPLICATION_ENTRY_ADDRESS=$(APPLICATION_ENTRY_ADDRESS)

# Define the path for include directories.
BCDS_INCLUDES = \
       -I$(BCDS_PACKAGE_HOME)/include \
       -I$(BCDS_PACKAGE_HOME)/source/protected \
       -I$(BCDS_PACKAGE_HOME)/source \
       -I$(BCDS_ESSENTIALS_PATH)/include \
       -I$(BCDS_ESSENTIALS_PATH)/include/bsp \
       -I$(BCDS_ESSENTIALS_PATH)/include/mcu \
       -I$(BCDS_ESSENTIALS_PATH)/include/mcu/stm32 \
       -I$(BCDS_CONFIG_PATH)/Essentials \
       -I$(BCDS_CONFIG_PATH)/stm32cube \
       -I$(BCDS_CONFIG_PATH) \
       -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/$(BCDS_STM32_DEVICE_TYPE)_HAL_Driver/Inc \
       -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/$(BCDS_STM32_DEVICE_TYPE)_HAL_Driver/Inc/Legacy \
       -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Include \
       -I$(BCDS_STM32CUBE_PATH)/3rd-party/stm32cube/Drivers/CMSIS/Device/ST/$(BCDS_STM32_DEVICE_TYPE)/Include \
       -I$(BCDS_STM32CUBE_PATH)/include

# Source and object files
BCDS_SOURCE_FILES = \
	$(wildcard source/*.c) \
	$(wildcard source/*.s) \
