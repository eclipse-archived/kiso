BCDS_UTILS_PATH = $(BCDS_PACKAGE_SHARED_HOME)/../utils
BCDS_STM32CUBE_PATH = $(BCDS_PACKAGE_SHARED_HOME)/../../thirdparty/stm32cubel4

#Includes required to run unit tests
BCDS_GTEST_INCLUDES = \
	$(BCDS_INCLUDES) \
	-I$(BCDS_STM32CUBE_PATH)/test/unit/include \
	-I./include/mcu/stm32 \
	-I./source/stm32/$(BCDS_MCU_FAMILY) \
	-I./test/unit/include \
	-I./test/unit/include/mcu \
	-I./test/unit/include/bsp \
	-I$(BCDS_UTILS_PATH)/test/unit/include

#Unit test files to be run
BCDS_GTEST_TEST_FILES = \
	test/unit/source/Assert_unittest.cc \
	test/unit/source/Retcode_unittest.cc

ifeq ($(BCDS_MCU_FAMILY),stm32l4)
	BCDS_GTEST_TEST_FILES += \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/FlashIntern_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/CRC_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/I2C_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/UART_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/RNG_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/SPI_unittest.cc

endif

ifeq ($(BCDS_MCU_FAMILY),stm32f7)
	BCDS_GTEST_TEST_FILES += \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/UART_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/CRC_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/I2C_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/RNG_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/SPI_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/FlashIntern_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/Watchdog_unittest.cc \
		test/unit/source/stm32/$(BCDS_MCU_FAMILY)/Timer_unittest.cc

endif
	

