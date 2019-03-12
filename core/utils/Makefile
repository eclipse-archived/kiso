BCDS_PACKAGE_NAME = Utils
BCDS_PACKAGE_ID = 2
export BCDS_PACKAGE_HOME = $(CURDIR)

# Mind that the variable BCDS_COMMON_MAKEFILE must be passed 
# from outside.
ifndef BCDS_COMMON_MAKEFILE
$(error "BCDS_COMMON_MAKEFILE not defined")
endif

# We include the common makefile. 
include $(BCDS_COMMON_MAKEFILE)

# We include the target specific makefile.
# Mind that the variable BCDS_TARGET_PLATFORM is defined 
# in the common makefile which has be included above. 
include $(BCDS_TARGET_PLATFORM).mk

# We include the gtest common makefile here. Note that the
# variable BCDS_COMMON_GTEST_MAKEFILE must be defined in the common 
# makefile which has been included above.
# the gtest common makefile defines the build rules
# gtest_build, gtest (executes the tests), gtest_clean
include $(BCDS_COMMON_GTEST_MAKEFILE)

include test/unit/gtest_$(BCDS_TARGET_PLATFORM).mk