#Includes required to run unit tests
BCDS_GTEST_INCLUDES = \
	$(BCDS_INCLUDES) \
	-I$(BCDS_STM32CUBE_PATH)/test/unit/include \
	-I$(BCDS_PACKAGE_HOME)/include/mcu/stm32 \
	-I$(BCDS_PACKAGE_HOME)/source/stm32/$(BCDS_MCU_FAMILY) \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include/mcu \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include/bsp \
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
	

