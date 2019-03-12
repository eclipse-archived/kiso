# This file has the static library paths of third-party software and BCDS platform
BCDS_FREERTOS_PATH = $(THIRD_PARTY_SHARED_PATH)/FreeRTOS
BCDS_EMLIB_PATH ?= $(THIRD_PARTY_SHARED_PATH)/emlib

BCDS_LIBS_LIST = \
    $(BCDS_ESSENTIALS_PATH);$(BCDS_ESSENTIALS_PATH)/debug/libEssentials_efm32_debug.a;$(BCDS_ESSENTIALS_PATH)/release/libEssentials_efm32.a \
    $(BCDS_TESTLING_PATH);$(BCDS_TESTLING_PATH)/debug/libTestling_efm32_debug.a;$(BCDS_TESTLING_PATH)/release/libTestling_efm32.a \
    $(BCDS_UTILS_PATH);$(BCDS_UTILS_PATH)/debug/libUtils_efm32_debug.a;$(BCDS_UTILS_PATH)/release/libUtils_efm32.a \
    $(BCDS_FREERTOS_PATH);$(BCDS_FREERTOS_PATH)/debug/libFreeRTOS_efm32_debug.a;$(BCDS_FREERTOS_PATH)/release/libFreeRTOS_efm32.a \
    $(BCDS_EMLIB_PATH);$(BCDS_EMLIB_PATH)/debug/libEMlib_efm32_debug.a;$(BCDS_EMLIB_PATH)/release/libEMlib_efm32.a \
    $(BCDS_BOARD_PATH);$(BCDS_BOARD_PATH)/debug/libBSP_efm32_debug.a;$(BCDS_BOARD_PATH)/release/libBSP_efm32.a

BCDS_LIBS_LIST += $(BCDS_TEST_LIBS_LIST)

define rule_template =
.PHONY: $(1)
$(1):
	$(MAKE) -C $(2) $(3) BCDS_COMMON_MAKEFILE=$(BCDS_COMMON_MAKEFILE)
endef

$(foreach lib_paths, $(BCDS_LIBS_LIST), \
	$(eval lib_paths_space_separated = $(subst ;, ,$(lib_paths))) \
	$(eval makefile_path = $(word 1,$(lib_paths_space_separated))) \
	$(eval debug_lib_path = $(word 2,$(lib_paths_space_separated))) \
	$(eval release_lib_path = $(word 3,$(lib_paths_space_separated))) \
	\
	$(eval BCDS_LIBS_DEBUG_PATH += $(dir $(debug_lib_path))) \
	$(eval BCDS_LIBS_DEBUG += $(debug_lib_path)) \
	$(eval $(call rule_template,$(debug_lib_path),$(makefile_path),debug)) \
	\
	$(eval BCDS_LIBS_RELEASE_PATH += $(dir $(release_lib_path))) \
	$(eval BCDS_LIBS_RELEASE += $(release_lib_path)) \
	$(eval $(call rule_template, $(release_lib_path), $(makefile_path), release)) \
)
