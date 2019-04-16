################### Common Makefile for STM32 ##################################

# The following variables are expected to be passed as input variables 
# from outside.
# BCDS_PACKAGE_NAME
# BCDS_PACKAGE_ID
# BCDS_PACKAGE_HOME
# BCDS_INCLUDES
# BCDS_SOURCE_FILES

include $(dir $(BCDS_COMMON_MAKEFILE))common_settings.mk

######################## Build Targets #######################################
# Generates Doxygen files and compresses it
.PHONY: doxygen
doxygen:
	@echo Building doxygen
	cd $(BCDS_PACKAGE_HOME)/doxygen && $(DOXYGEN) > doxygenDump.txt
	cd $(BCDS_PACKAGE_HOME)/doxygen && $(HHC) html/index.hhp
	@echo Documentation ready

# Deletes the generated doxygen files
doxygen_clean:
	@echo Cleaning doxygen
	rm -rf $(BCDS_PACKAGE_HOME)/doxygen/doxygenWarning.log
	rm -rf $(BCDS_PACKAGE_HOME)/doxygen/doxygenDump.txt
	rm -rf $(BCDS_PACKAGE_HOME)/doxygen/*.tmp
	rm -rf $(BCDS_PACKAGE_HOME)/doxygen/html
	@echo Documentation cleaned

######################## Build Targets #######################################
.PHONY: check
check::

ifndef BCDS_PACKAGE_NAME
	$(error BCDS_PACKAGE_NAME not defined)
endif

ifndef BCDS_PACKAGE_ID
	$(error BCDS_PACKAGE_ID not defined)
endif

ifndef BCDS_PACKAGE_HOME
	$(error BCDS_PACKAGE_HOME not defined)
endif

ifeq ($$(BCDS_INCLUDES),)
	$(error BCDS_INCLUDES not defined)
endif

ifeq ($$(BCDS_SOURCE_FILES),)
	$(error BCDS_SOURCE_FILES not defined)
endif

.PHONY: diagnosis
diagnosis::
	@echo "BCDS_PACKAGE_NAME:      "$(BCDS_PACKAGE_NAME)
	@echo "BCDS_PACKAGE_HOME:      "$(BCDS_PACKAGE_HOME)
	@echo "BCDS_PACKAGE_ID:        "$(BCDS_PACKAGE_ID)
	@echo "BCDS_TARGET_PLATFORM:   "$(BCDS_TARGET_PLATFORM)
	@echo "BCDS_ARCH_CPU:          "$(BCDS_ARCH_CPU)
	@echo "BCDS_SHARED_PATH:       "$(BCDS_SHARED_PATH)
	@echo "THIRD_PARTY_SHARED_PATH:"$(THIRD_PARTY_SHARED_PATH)
	@echo "BCDS_TOOLS_PATH:        "$(BCDS_TOOLS_PATH)
	@echo "BCDS_CONFIG_PATH:       "$(BCDS_CONFIG_PATH)
	@echo "BCDS_DEBUG_PATH:        "$(BCDS_DEBUG_PATH)
	@echo "BCDS_DEBUG_OBJECT_PATH: "$(BCDS_DEBUG_OBJECT_PATH)
	@echo "BCDS_RELEASE_PATH:      "$(BCDS_RELEASE_PATH)
	@echo "BCDS_DEBUG_LIB:         "$(BCDS_DEBUG_LIB)
	@echo "BCDS_RELEASE_LIB:       "$(BCDS_RELEASE_LIB)
	@echo "BCDS_LIBS_RELEASE:      "$(BCDS_LIBS_RELEASE)
	@echo "BCDS_LIBS_DEBUG:        "$(BCDS_LIBS_DEBUG)
	@echo "BCDS_INCLUDES:          "
	@echo $(BCDS_INCLUDES)
	@echo ""
	@echo "BCDS_SOURCE_FILES:      "
	@echo $(BCDS_SOURCE_FILES)
	@echo ""
	@echo "BCDS_DEBUG_OBJECT_FILES: "
	@echo $(BCDS_DEBUG_OBJECT_FILES)
	@echo ""
	@echo "BCDS_CFLAGS_DEBUG:      "$(BCDS_CFLAGS_DEBUG)
	@echo "CFLAGS_DEBUG:           "$(CFLAGS_DEBUG)
	@echo "BCDS_CFLAGS_RELEASE:    "$(BCDS_CFLAGS_RELEASE)
	@echo "CFLAGS_RELEASE:         "$(CFLAGS_RELEASE)

.PHONY: debug
debug: check $(BCDS_DEBUG_LIB)

.PHONY: release	
release: check $(BCDS_RELEASE_LIB)

$(BCDS_DEBUG_LIB): $$(BCDS_DEBUG_OBJECT_FILES)
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $(BCDS_DEBUG_OBJECT_FILES)

$(BCDS_RELEASE_LIB): $$(BCDS_RELEASE_OBJECT_FILES)
	mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $(BCDS_RELEASE_OBJECT_FILES)

# compile C files
$(BCDS_DEBUG_OBJECT_PATH)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS_DEBUG) -I . $(BCDS_INCLUDES) $< -o $@
	
$(BCDS_RELEASE_OBJECT_PATH)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS_RELEASE) -I . $(BCDS_INCLUDES) $< -o $@

$(BCDS_DEBUG_OBJECT_PATH)/%.o: %.s
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS_DEBUG) -I . $(BCDS_INCLUDES) $< -o $@
	
$(BCDS_RELEASE_OBJECT_PATH)/%.o: %.s
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS_RELEASE) -I . $(BCDS_INCLUDES) $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning Application files"
	rm -rf $(BCDS_DEBUG_PATH) $(BCDS_RELEASE_PATH)

.PHONY: cdt
cdt:
	$(CC) $(CFLAGS_DEBUG) -O0 $(BCDS_INCLUDES) -E -P -v -dD -c ${CDT_INPUT_FILE}

.PHONY: no_targets__ list
no_targets__:
list:
	sh -c "$(MAKE) -p no_targets__ | awk -F':' '/^[a-zA-Z0-9][^\$$#\/\\t=]*:([^=]|$$)/ {split(\$$1,A,/ /);for(i in A)print A[i]}' | grep -v '__\$$' | sort"	