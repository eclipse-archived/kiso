# Note that we require here the variable BCDS_TEST_SECTION_PATH to be defined.
# This variable is usually defined bt the makefile which includes the 
# TestSection.mk. The value of this variable must be the absolute path to 
# the folder which contains the Test Section, i.e. TestSection.c and TestSection.mk

# This is the path to the folder which contains the Test Suites. We know in this case
# that the Test Suites are located in the parent folder of the Test Section itself.
BCDS_TEST_PATH=$(BCDS_TEST_SECTION_PATH)/..

BCDS_DRIVERS_PATH ?= $(BCDS_SHARED_PATH)/Drivers
BCDS_TESTLING_PATH ?= $(BCDS_SHARED_PATH)/Testing/Testling

# We must define the list of test source files.
# For every file, we must state the path starting with BCDS_TEST_SECTION_PATH. 
BCDS_TEST_SOURCE_FILES = \
	$(BCDS_TEST_PATH)/TestSection/TestSection.c \
	$(wildcard $(BCDS_TEST_PATH)/TestSuite_ErrorLogger/*.c)

BCDS_TEST_INCLUDES += \
	-I$(BCDS_TEST_PATH) \
	-I$(BCDS_TEST_PATH)/TestSuite_ErrorLogger \
	-I$(BCDS_SHARED_PATH)/Utils/include \
	-I$(BCDS_DRIVERS_PATH)/include \
	-I$(BCDS_CONFIG_PATH)/Drivers \
	-I$(BCDS_CONFIG_PATH)/Testling/NVM/generated 

# You should add here the libraries you depend on
BCDS_TEST_LIBS_LIST += \
	$(BCDS_DRIVERS_PATH);$(BCDS_DRIVERS_PATH)/debug/libDrivers_efm32_debug.a;$(BCDS_DRIVERS_PATH)/release/libDrivers_efm32.a

#	path_to_dir_of_makefile;path_to_debug_bin;path_to_release_bin
# for example:
#	$(BCDS_SHARED_PATH)/Basics;$(BCDS_SHARED_PATH)/Basics/debug/libBasics_$(BCDS_TARGET_PLATFORM)_debug.a;$(BCDS_SHARED_PATH)/Basics/release/libBasics_$(BCDS_TARGET_PLATFORM).a
# you need to make sure that you exported the path correctly

BCDS_TEST_MACROS = \
-D BCDS_FEATURE_TEST_REBOOT=1 \
-D TEST_MAX_NUMBER_OF_TEST_SUITES=1 \
-D TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE=10