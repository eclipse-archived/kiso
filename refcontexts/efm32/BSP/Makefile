BCDS_PACKAGE_NAME = BSP
BCDS_PACKAGE_ID = 10

BCDS_PACKAGE_VERSION_MAJOR =  0
BCDS_PACKAGE_VERSION_MINOR =  5
BCDS_PACKAGE_VERSION_PATCH =  1

BCDS_PACKAGE_HOME = $(CURDIR)

# We include the common makefile. 
# Mind that the variable BCDS_COMMON_MAKEFILE must be passed 
# from outside.
include $(BCDS_COMMON_MAKEFILE)

# We include the target specific makefile.
# Mind that the variable BCDS_TARGET_PLATFORM is defined 
# in the common makefile which has be included above. 
include $(BCDS_TARGET_PLATFORM).mk
