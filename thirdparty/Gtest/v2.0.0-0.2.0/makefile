# This makefile is intended to be called by project specific unit testing makefiles.
# 
# Makefile targets:
#     all - generates precompiled headers and the gtest library
#     clean - cleans up the gtest source directory

#SHELL = cmd

# Bin utils
STRIP = strip

# Flags passed to the preprocessor.
CPPFLAGS += -I.

# Flags passed to the C++ compiler.
CXXFLAGS += -O0 -g3 -static -Wall -Wextra -Winvalid-pch -fno-exceptions

# Source files
GTEST_SOURCES = gtest-all.cc gtest_main.cc
GTEST_HEADERS = fff.h gtest.h

# Path to output directory - can be overridden from command line
# example > gmake makefile GTEST_OUTPUT_DIR=c:/gmake all
GTEST_OUTPUT_DIR ?= .

# Target library
GTEST_LIB = libgtest.a

HPP_OBJS := $(foreach srcfile,$(GTEST_HEADERS), $(addsuffix .gch, $(srcfile)))
CPP_OBJS_GTEST := $(foreach srcfile,$(GTEST_SOURCES), $(addsuffix .o, $(basename $(srcfile))))

# setup search paths for GNU make
vpath %.h.gch $(GTEST_OUTPUT_DIR)
vpath %.o $(GTEST_OUTPUT_DIR)
vpath %.a $(GTEST_OUTPUT_DIR)

# Build targets
.PHONY: all
all: $(GTEST_OUTPUT_DIR) $(HPP_OBJS) $(GTEST_LIB)

# Create directories
$(GTEST_OUTPUT_DIR) : 
	-@mkdir $@

$(GTEST_LIB) : $(CPP_OBJS_GTEST)
	$(AR) $(ARFLAGS) $(GTEST_OUTPUT_DIR)/$@ $(foreach srcfile, $(^F), $(GTEST_OUTPUT_DIR)/$(srcfile))
	$(STRIP) --strip-debug $(GTEST_OUTPUT_DIR)/$@

%.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o"$(GTEST_OUTPUT_DIR)/$@" "$<"

.PRECIOUS: %.h.gch
%.h.gch: %.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o"$(GTEST_OUTPUT_DIR)/$@" "$<"

.PHONY: clean
clean:
	-$(RM) -v $(GTEST_OUTPUT_DIR)/*.gch $(GTEST_OUTPUT_DIR)/*.o $(GTEST_OUTPUT_DIR)/lib*.a

# Use this target to upgrade from outdated incompatible MinGW/GCC versions to the latest one (4.7.2 at the implementation of this target)
.PHONY: mingw_upgrade
mingw_upgrade:
	@mingw-get update
	@mingw-get upgrade
