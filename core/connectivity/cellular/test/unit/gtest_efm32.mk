# Include paths of BCDS shared and 3rd party shared packages
BCDS_GTEST_INCLUDES = \
	$(BCDS_INCLUDES) \
	-I$(BCDS_PACKAGE_HOME)/source/common \
	-I$(BCDS_PACKAGE_HOME)/source/protected \
	-I$(BCDS_PACKAGE_HOME)/source/$(BCDS_CELLULAR_PLATFORM) \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include \
	-I$(BCDS_PACKAGE_HOME)/test/unit/source/protected \
	-I$(BCDS_UTILS_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/bsp \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/mcu \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/mcu/$(BCDS_TARGET_PLATFORM) \
	-I$(BCDS_EMLIB_PATH)/test/unit/include \
	-I$(BCDS_FREERTOS_PATH)/test/unit/include

# Unit test files to be run
BCDS_GTEST_TEST_FILES = \
    test/unit/source/common/AtResponseParser_unittest.cc \
    test/unit/source/common/Utils_unittest.cc \
    test/unit/source/$(BCDS_CELLULAR_PLATFORM)/Network_unittest.cc \
    test/unit/source/common/Engine_unittest.cc \
    test/unit/source/$(BCDS_CELLULAR_PLATFORM)/Socket_unittest.cc \

ifeq ($(BCDS_CELLULAR_PLATFORM),ublox)
BCDS_GTEST_TEST_FILES += \
    test/unit/source/ublox/Dns_unittest.cc
endif
