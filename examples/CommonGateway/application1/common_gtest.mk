#####################################################################################
# Execution of all unitest in a common place.
#####################################################################################

# Need to include all the project build variables
BCDS_GTEST_VERSION ?= v2.0.0-0.2.0
BCDS_GTEST_PATH = $(BCDS_TOOLS_PATH)/gtest/$(BCDS_GTEST_VERSION)

# Input and Output Directories
BCDS_GTEST_CODE_PATH = $(BCDS_PACKAGE_SHARED_HOME)/test/unit
BCDS_GTEST_OUTPUT_PATH = $(BCDS_DEBUG_PATH)/gtest
BCDS_LCOV_OUTPUT_PATH ?= $(BCDS_GTEST_OUTPUT_PATH)/coverage

# Object files
BCDS_GTEST_OBJECT_FILES = $(patsubst %_unittest.cc,$(BCDS_GTEST_OUTPUT_PATH)/%_unittest.o, $(BCDS_GTEST_TEST_FILES))
BCDS_GTEST_EXECUTORS = $(patsubst %_unittest.cc,$(BCDS_GTEST_OUTPUT_PATH)/%_unittest.exe, $(BCDS_GTEST_TEST_FILES))

# Flags passed to the C++ compiler.
CXXFLAGS = -O0 -g3 -static -Wall -Wextra -Winvalid-pch -Wswitch-default -fno-exceptions -save-temps=obj
BCDS_GCOV_FLAGS = -fprofile-arcs -ftest-coverage
BCDS_GTEST_LIBS = -lgcov -lgtest

#Create gtest library
libgtest.a:
	@echo "Creating the gtest library"
	$(MAKE) -C $(BCDS_GTEST_PATH) all

#Execute unittests
.PHONY: gtest
gtest:: gtest_build
	$(foreach BCDS_gtest_exe,$(BCDS_GTEST_EXECUTORS),\
		$(BCDS_gtest_exe) --gtest_output=xml:$(BCDS_GTEST_OUTPUT_PATH)/$(basename $(notdir $(BCDS_gtest_exe))).xml || 0;)

.PHONY: gtest_build
gtest_build: libgtest.a $$(BCDS_GTEST_EXECUTORS)

$(BCDS_GTEST_OUTPUT_PATH)/%_unittest.exe:  $(BCDS_GTEST_OUTPUT_PATH)/%_unittest.o
	$(CXX) $(CXXFLAGS) $(BCDS_GCOV_FLAGS) $(BCDS_MACROS_DEBUG) -L $(BCDS_GTEST_PATH) $(BCDS_GTEST_LIBS) $(@D)/$(basename $(notdir $@)).o -o $@

# Rule to build the unittest files
$(BCDS_GTEST_OUTPUT_PATH)/%_unittest.o: $(BCDS_PACKAGE_SHARED_HOME)/%_unittest.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(BCDS_GCOV_FLAGS) $(BCDS_MACROS_DEBUG) -I $(BCDS_GTEST_PATH) $(BCDS_GTEST_INCLUDES) -c $< -o $@

LCOV_ERROR_FILE = $(BCDS_GTEST_OUTPUT_PATH)/lcov.err
LCOV_INFO_FILE = $(BCDS_GTEST_OUTPUT_PATH)/coverage.info

# HTML coverage report generator
.PHONY: lcov

lcov:: 
	@sh -i -c "(\
		lcov --rc lcov_branch_coverage=1 --capture --directory $(BCDS_GTEST_OUTPUT_PATH) --output-file coverage.info 2>lcov.err && \
		lcov --rc lcov_branch_coverage=1 --remove coverage.info gtest.h --output-file coverage.info 2>$(LCOV_ERROR_FILE) && \
		lcov --rc lcov_branch_coverage=1 --remove coverage.info *.hh --output-file coverage.info 2>$(LCOV_ERROR_FILE) && \
		lcov --rc lcov_branch_coverage=1 --remove coverage.info *.cc --output-file coverage.info 2>$(LCOV_ERROR_FILE) && \
		lcov --rc lcov_branch_coverage=1 --remove coverage.info *.h --output-file coverage.info 2>$(LCOV_ERROR_FILE) && \
		lcov --rc lcov_branch_coverage=1 --remove coverage.info gthr-default.h atomicity.h new_allocator.h type_traits.h allocator.h basic_string.h basic_string.tcc char_traits.h ios_base.h stl_iterator_base_funcs.h stl_iterator_base_types.h iostream sstream --output-file $(LCOV_INFO_FILE) 2>$(LCOV_ERROR_FILE) && \
		genhtml $(LCOV_INFO_FILE) --rc lcov_branch_coverage=1 --demangle-cpp --ignore-errors source --output-directory $(BCDS_LCOV_OUTPUT_PATH) 2>$(LCOV_ERROR_FILE)) || (\
		echo \"WARNING: did you run gtest before?\" && \
		exit 1)"

.PHONY: lcov_clean

lcov_clean::
	$(RM) lcov.err
	$(RM) coverage.info


#Clean unittest
.PHONY: gtest_clean
gtest_clean::
	$(RM) -rf $(BCDS_GTEST_OUTPUT_PATH)
