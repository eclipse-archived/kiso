# Include paths of BCDS shared and 3rd party shared packages
BCDS_GTEST_INCLUDES = \
	$(BCDS_INCLUDES) \
	-I$(BCDS_PACKAGE_HOME)/source \
	-I$(BCDS_PACKAGE_HOME)/source/common \
	-I$(BCDS_PACKAGE_HOME)/source/protected \
	-I$(BCDS_PACKAGE_HOME)/source/$(BCDS_CELLULAR_PLATFORM) \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include \
	-I$(BCDS_PACKAGE_HOME)/test/unit/source \
	-I$(BCDS_PACKAGE_HOME)/test/unit/source/protected \
	-I$(BCDS_UTILS_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/bsp \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/mcu \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/mcu/$(BCDS_TARGET_PLATFORM) \
	-I$(BCDS_FREERTOS_PATH)/test/unit/include

# Unit test files to be run
BCDS_GTEST_TEST_FILES = \
	test/unit/source/common/AT_3GPP_27007_unittest.cc \
	test/unit/source/common/AtResponseParser_unittest.cc \
	test/unit/source/common/AtUtils_unittest.cc \
	test/unit/source/$(BCDS_CELLULAR_PLATFORM)/AT_UBlox_unittest.cc \
	test/unit/source/ModemEmulator_unittest.cc \

BCDS_MACROS_DEBUG += \
	-D __STDC_FORMAT_MACROS
