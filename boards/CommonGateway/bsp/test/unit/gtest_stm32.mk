#####################################################################
# Directory of the package source code (not the tests)
#####################################################################

# The directory paths of the C source files that needs to be tested 
BCDS_GTEST_SOURCE_DIRS = \
	-I$(BCDS_PACKAGE_HOME)/include \
	-I$(BCDS_PACKAGE_HOME)/source \
	-I$(BCDS_PACKAGE_HOME)/source/protected \
	-I$(BCDS_PACKAGE_HOME)/source/include

# Include paths of platform and libraries
BCDS_GTEST_INCLUDES = \
	$(BCDS_INCLUDES) \
	$(BCDS_GTEST_SOURCE_DIRS) \
	-I$(BCDS_PACKAGE_HOME)/test/unit/include \
	-I$(BCDS_PACKAGE_HOME)/test/unit/source/protected \
	-I$(BCDS_STM32CUBE_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/bsp \
	-I$(BCDS_ESSENTIALS_PATH)/test/unit/include/mcu \
	-I$(BCDS_UTILS_PATH)/test/unit/include \
	
#Unit test files to be run
BCDS_GTEST_TEST_FILES = \
