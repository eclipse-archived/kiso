# Note that we require here the variable BCDS_TEST_SECTION_PATH to be defined.
# This variable is usually defined bt the makefile which includes the 
# TestSection.mk. The value of this variable must be the absolute path to 
# the folder which contains the Test Section, i.e. TestSection.c and TestSection.mk

# This is the path to the folder which contains the Test Suites. We know in this case
# that the Test Suites are located in the parent folder of the Test Section itself.
BCDS_TEST_PATH = $(BCDS_TEST_SECTION_PATH)/..

BCDS_BSTLIB_PATH ?= $(THIRD_PARTY_SHARED_PATH)/BSTLib

# We must define the list of test source files.
# For every file, we must state the path starting with BCDS_TEST_SECTION_PATH. 
BCDS_TEST_SOURCE_FILES = \
	$(BCDS_TEST_PATH)/TestSection/TestSection.c \
	$(BCDS_TEST_PATH)/TestSuite_UART/TestSuite_UART.c

ifeq ($(BCDS_TARGET_PLATFORM),efm32)	
BCDS_TEST_SOURCE_FILES += \
	$(BCDS_TEST_PATH)/TestSuite_I2C/TestSuite_I2C.c \
	$(BCDS_TEST_PATH)/TestSuite_SPI/TestSuite_SPI.c \
	$(BCDS_TEST_PATH)/TestSuite_TIMER/TestSuite_TIMER.c
endif

BCDS_TEST_INCLUDES = \
	-I$(BCDS_BSTLIB_PATH)/3rd-party/bstlib/BMM050_driver \
	-I$(BCDS_BOARD_PATH)/source \
	-I$(BCDS_BOARD_PATH)/include \
	-I$(BCDS_TEST_PATH) \
	-I$(BCDS_TEST_PATH)/TestSuite_I2C \
	-I$(BCDS_TEST_PATH)/TestSuite_SPI \
	-I$(BCDS_TEST_PATH)/TestSuite_UART \
	-I$(BCDS_TEST_PATH)/TestSuite_TIMER \
	-I$(BCDS_TEST_PATH)/TestSuite_Sleep

# You should add here the 3rd party libraries you depend on
ifeq ($(BCDS_TARGET_PLATFORM),efm32)	
BCDS_TEST_LIBS_LIST += \
	$(BCDS_BSTLIB_PATH);$(BCDS_BSTLIB_PATH)/debug/libBSTLib_efm32_debug.a;$(BCDS_BSTLIB_PATH)/release/libBSTLib_efm32.a
endif

#	path_to_dir_of_makefile;path_to_debug_bin;path_to_release_bin
