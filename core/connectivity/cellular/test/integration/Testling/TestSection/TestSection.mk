# Note that we require here the variable BCDS_TEST_SECTION_PATH to be defined.
# This variable is usually defined bt the makefile which includes the 
# TestSection.mk. The value of this variable must be the absolute path to 
# the folder which contains the Test Section, i.e. TestSection.c and TestSection.mk

# This is the path to the folder which contains the Test Suites. We know in this case
# that the Test Suites are located in the parent folder of the Test Section itself.
BCDS_TEST_PATH=$(BCDS_TEST_SECTION_PATH)/..

BCDS_CCHANNEL = USB_CCHANNEL

# We must define the list of test source files.
# For every file, we must state the path starting with BCDS_TEST_SECTION_PATH. 
BCDS_TEST_SOURCE_FILES = \
	$(BCDS_TEST_PATH)/Testsection/TestSection.c \
	$(BCDS_TEST_PATH)/TestSuite_Support/CellularSupport.c \
	$(BCDS_TEST_PATH)/TestSuite_Load/LoadTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_MultiTaskHandle/MultiTaskHandleTest.c \
	$(BCDS_TEST_PATH)/TestSuite_Negative/NegativeTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_Network/NetworkTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_PowerControl/PowerControlTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_Query/QueryTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_Smoke/SmokeTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_Sms/SmsTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_Socket/SocketTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_Stress/StressTestSuite.c \
	$(BCDS_TEST_PATH)/TestSuite_Udp/UdpTestSuite.c
	
BCDS_TEST_INCLUDES = \
	-I$(BCDS_CONFIG_PATH)/Cellular \
	-I$(BCDS_CONFIG_PATH)/Utils \
	-I$(BCDS_SHARED_PATH)/Cellular/include \
	-I$(BCDS_SHARED_PATH)/testing/Testling/include \
	-I$(BCDS_SHARED_PATH)/testing/Testling/source \
	-I$(BCDS_TEST_PATH) \
	-I$(BCDS_TEST_PATH)/TestSuite_PowerControl \
	-I$(BCDS_TEST_PATH)/TestSuite_Support \
	-I$(BCDS_TEST_PATH)/TestSuite_Socket \
	-I$(BCDS_TEST_PATH)/TestSuite_Network \
	-I$(BCDS_TEST_PATH)/TestSuite_Negative \
	-I$(BCDS_TEST_PATH)/TestSuite_Query \
	-I$(BCDS_TEST_PATH)/TestSuite_Smoke \
	-I$(BCDS_TEST_PATH)/TestSuite_Stress \
	-I$(BCDS_TEST_PATH)/TestSuite_MultiTaskHandle \
	-I$(BCDS_TEST_PATH)/TestSuite_Load \
	-I$(BCDS_TEST_PATH)/TestSuite_Udp \
	-I$(BCDS_TEST_PATH)/TestSuite_Sms \
	-I$(BCDS_TEST_PATH)/TestSuites \

# You should add here the libraries you depend on
BCDS_TEST_LIBS_LIST += \
	$(BCDS_SHARED_PATH)/Cellular;$(BCDS_SHARED_PATH)/Cellular/debug/libCellular_efm32_debug.a;$(BCDS_SHARED_PATH)/Cellular/release/libCellular_efm32.a \
	#	path_to_dir_of_makefile;path_to_debug_bin;path_to_release_bin

# Add here the specific config needed by the module
BCDS_TEST_MACROS = \
	-D TEST_MAX_NUMBER_OF_TEST_SUITES=11 \
	-D TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE=50 \

ifeq ($(BCDS_CELLULAR_PLATFORM),ublox)
BCDS_TEST_MACROS += -D CELLULAR_SOCKET_COUNT=7
BCDS_TEST_MACROS += -D BCDS_FEATURE_BSP_UMTS_LISAU2=1
BCDS_TEST_MACROS += -D BCDS_FEATURE_BSP_UMTS_QUECTELUC20=0
endif
ifeq ($(BCDS_CELLULAR_PLATFORM),quectel)
BCDS_TEST_MACROS += -D CELLULAR_SOCKET_COUNT=12
BCDS_TEST_MACROS += -D BCDS_FEATURE_BSP_UMTS_QUECTELUC20=1
BCDS_TEST_MACROS += -D BCDS_FEATURE_BSP_UMTS_LISAU2=0
endif

ifdef BCDS_ECHO_SERVER_IP
BCDS_TEST_MACROS += -D BCDS_ECHO_SERVER_IP=$(BCDS_ECHO_SERVER_IP)
endif
ifdef BCDS_ECHO_SERVER_PORT
BCDS_TEST_MACROS += -D BCDS_ECHO_SERVER_PORT=$(BCDS_ECHO_SERVER_PORT)
endif
ifdef BCDS_NETWORK_APN
BCDS_TEST_MACROS += -D BCDS_NETWORK_APN=$(BCDS_NETWORK_APN)
endif

ifeq ($(BCDS_TARGET_PLATFORM),efm32)
BCDS_TEST_MACROS += \
-D BCDS_FEATURE_DMA=1
endif