# Include paths of BCDS shared and 3rd party shared packages
BCDS_GTEST_INCLUDES = \
	$(BCDS_INCLUDES) \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include \
	-Isource \
	-I$(THIRD_PARTY_SHARED_PATH)/FreeRTOS/test/unit/include \
	-I$(BCDS_UTILS_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/mcu \
	-I$(BCDS_PACKAGE_HOME)/source/Carbon  \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include \
	-I$(BCDS_PACKAGE_HOME)/include \
	-I$(BCDS_CONFIG_PATH)/ \
	-I$(BCDS_CONFIG_PATH)/Utils \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/source/portable/ARM_CM3 \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/source/portable/GCC/ARM_CM3 \
	-I$(BCDS_FREERTOS_PATH)/3rd-party/FreeRTOS/source/include \
	-I$(BCDS_FREERTOS_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include

# Unit test files to be run
BCDS_GTEST_TEST_FILES = \
	test/unit/source/XProtocol_unittest.cc \
	test/unit/source/CRC_unittest.cc \
	test/unit/source/LeanB2CAP_unittest.cc \
	test/unit/source/RingBuffer_unittest.cc \
	test/unit/source/UARTTransceiver_unittest.cc \
	test/unit/source/TLV_unittest.cc \
	test/unit/source/Carbon/CmdProcessor_unittest.cc \
	test/unit/source/Carbon/GuardedTask_unittest.cc \
	test/unit/source/Carbon/TaskMonitor_unittest.cc \
	test/unit/source/Carbon/Queue_unittest.cc \
	test/unit/source/Carbon/EventHub_unittest.cc \
	test/unit/source/Carbon/PipeAndFilter_unittest.cc