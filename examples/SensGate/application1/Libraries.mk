export BCDS_COMMON_MAKEFILE=$(CURDIR)/common.mk

# List of build targets
BUILDS := debug release
# List of additional libraries
# If the user wants to add a new lib Abc, then Abc needs to be added to this list.
# In order for this to work, it is expected that parent make files define the library 
# path in a variable called BCDS_ABC_PATH (note the upper case of the libary name
# The compilations results in a .a lib in the following location and naming convension
# $(BCDS_$(2)_PATH)/libAbc_$(BCDS_TARGET_PLATFORM)_$(build).a
LIBS := Utils Essentials BSP STM32CubeL4 FreeRTOS

#1 is the build 2 is the lib
define add_lib =
# This is the aggregated build path construction based on the build
$(eval AGGR_BUILD_PATH := $(shell echo BCDS_LIBS_$(1)_PATH | tr  '[:lower:]' '[:upper:]'))
# This is the aggregated libraries with absolute paths construction based on the build
$(eval BUILD_LIBS_WITH_PATH := $(shell echo BCDS_LIBS_$(1) | tr  '[:lower:]' '[:upper:]'))
## This is the externally defined library path construction based on the library
$(eval LIB_BUILD_PATH_VAR := $(value $(shell echo BCDS_$(2)_PATH | tr  '[:lower:]' '[:upper:]')))
#LIB_BUILD_PATH = $(LIB_BUILD_PATH_VAR)
$(eval LIB_OUT_NAME := $(shell echo lib$(2)_$(BCDS_TARGET_PLATFORM)_$(1).a))
$$(AGGR_BUILD_PATH) += $(LIB_BUILD_PATH_VAR)/$(1)
$$(BUILD_LIBS_WITH_PATH) += $(LIB_BUILD_PATH_VAR)/$(1)/$(LIB_OUT_NAME)

$(eval LIB_TARGET := $(LIB_BUILD_PATH_VAR)/$(1)/$(LIB_OUT_NAME))
$(LIB_TARGET)::
	 $(MAKE) -C $(LIB_BUILD_PATH_VAR) $(1) BCDS_COMMON_MAKEFILE=$(BCDS_COMMON_MAKEFILE)
endef

$(foreach build,$(BUILDS), $(foreach library,$(LIBS), $(eval $(call add_lib,$(build),$(library)))))


# The library paths used for the clean job are aggregated in BCDS_LIBS_$(BUILD)_PATH
# The library absolute path +name are are aggregated in BCDS_LIBS_$(BUILD)