#Includes required to run unit tests
BCDS_GTEST_INCLUDES = \
	$(BCDS_INCLUDES) \
	-I$(BCDS_STM32CUBE_PATH)/test/unit/include \
	-I$(BCDS_PACKAGE_HOME)/include/mcu/ti_cc26xx \
	-I$(BCDS_PACKAGE_HOME)/source/ti_cc26xx \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include/mcu \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include/bsp \
	-I$(BCDS_UTILS_PATH)/test/unit/include

#Unit test files to be run
BCDS_GTEST_TEST_FILES = \
	test/unit/source/ti_cc26xx/UART_unittest.cc \
	test/unit/source/ti_cc26xx/SPI_unittest.cc\
	test/unit/source/ti_cc26xx/I2C_unittest.cc \
	test/unit/source/Assert_unittest.cc \
	test/unit/source/Retcode_unittest.cc 
