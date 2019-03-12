#Includes required to run unit tests
BCDS_GTEST_INCLUDES = \
	$(BCDS_INCLUDES) \
	-I $(BCDS_PACKAGE_HOME)/include/efm32 \
	-I $(BCDS_PACKAGE_HOME)/source \
	-I $(BCDS_PACKAGE_HOME)/source/efm32 \
	-I $(BCDS_SHARED_PATH)/Essentials \
	-I $(BCDS_PACKAGE_HOME)/test/unit/include \
	-I $(BCDS_PACKAGE_HOME)/test/unit/include/BSP \
	-I $(BCDS_PACKAGE_HOME)/test/unit/include/MCU \
	-I $(BCDS_EMLIB_PATH)/test/unit/include \

#Unit test files to be run
BCDS_GTEST_TEST_FILES = \
	test/unit/source/efm32/SPI/SPI_unittest.cc \
	test/unit/source/efm32/LEUART/LEUART_unittest.cc \
	test/unit/source/efm32/UART/UART_unittest.cc \
	test/unit/source/efm32/Flash/Flash_unittest.cc \
	test/unit/source/efm32/I2C/I2C_unittest.cc \
	test/unit/source/efm32/Timer_unittest.cc \
	test/unit/source/efm32/Watchdog_unittest.cc \
	test/unit/source/Assert_unittest.cc \
	test/unit/source/Retcode_unittest.cc
