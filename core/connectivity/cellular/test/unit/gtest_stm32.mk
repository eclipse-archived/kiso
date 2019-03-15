#Paths to BCDS shared packages on which this package depends
BCDS_ESSENTIALS_PATH ?= $(BCDS_SHARED_PATH)/Essentials
BCDS_UTILS_PATH ?= $(BCDS_SHARED_PATH)/Utils
BCDS_PERIPHERALS_PATH ?= $(BCDS_SHARED_PATH)/Peripherals

BCDS_FREERTOS_LIB_PATH ?= $(THIRD_PARTY_SHARED_PATH)/FreeRTOS

# Include paths of platform and libraries
BCDS_GTEST_INCLUDES = \
    -I$(BCDS_PACKAGE_HOME)/source/common \
    -I$(BCDS_PACKAGE_HOME)/source/protected \
    -I$(BCDS_PACKAGE_HOME)/source/$(BCDS_CELLULAR_PLATFORM) \
    -I$(BCDS_ESSENTIALS_PATH)/include \
    -I$(BCDS_PERIPHERALS_PATH)/include \
    -I$(BCDS_FREERTOS_LIB_PATH)/3rd-party/FreeRTOS/source/include \
    -I$(BCDS_FREERTOS_LIB_PATH)/3rd-party/FreeRTOS/source/portable/GCC/ARM_CM4F \
    -I$(BCDS_UTILS_PATH)/include \
    -I$(BCDS_PACKAGE_HOME)/test/unit/include \
    -I$(BCDS_PACKAGE_HOME)/test/unit/source/protected \
    -I$(BCDS_SHARED_PATH)/RefConfig \
    -I$(BCDS_FREERTOS_LIB_PATH)/test/unit/include \
    -I$(BCDS_ESSENTIALS_PATH)/test/unit/include \
    -I$(BCDS_PERIPHERALS_PATH)/test/unit/include/EM \
    -I$(BCDS_PERIPHERALS_PATH)/test/unit/include \
    -I$(BCDS_SHARED_PATH)/RefContexts/stm32/config/Utils \


# Unit test files to be run
BCDS_GTEST_TEST_FILES = \
    test/unit/source/common/AtResponseParser_unittest.cc \
    test/unit/source/common/Engine_unittest.cc \
    test/unit/source/common/Utils_unittest.cc \
    test/unit/source/$(BCDS_CELLULAR_PLATFORM)/Network_unittest.cc \
    test/unit/source/$(BCDS_CELLULAR_PLATFORM)/Socket_unittest.cc


ifeq ($(BCDS_CELLULAR_PLATFORM),ublox)
BCDS_GTEST_TEST_FILES += \
	test/unit/source/ublox/Dns_unittest.cc
endif
