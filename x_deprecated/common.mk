	# /*
# -------------------------------------------------------------------------------
# This file is part of nsg-library.
# http://nsg-library.googlecode.com/

# Copyright (c) 2014-2015 Néstor Silveira Gorski

# -------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.

# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:

# 1. The origin of this software must not be misrepresented; you must not
# claim that you wrote the original software. If you use this software
# in a product, an acknowledgment in the product documentation would be
# appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
# misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
# -------------------------------------------------------------------------------
# */
THIS_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
NACL_SDK_ROOT ?= $(abspath $(dir $(THIS_MAKEFILE))../..)

#
# Disable DOS PATH warning when using Cygwin based tools Windows
#
CYGWIN ?= nodosfilewarning
export CYGWIN

###################################################################################

PNACL_TC_PATH := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_pnacl)
TOOLCHAIN_CXX := $(PNACL_TC_PATH)/bin/pnacl-clang++
TOOLCHAIN_AR := $(PNACL_TC_PATH)/bin/pnacl-ar
TOOLCHAIN_FINALIZE := $(PNACL_TC_PATH)/bin/pnacl-finalize
LDFLAGS_RELEASE := -L$(NACL_SDK_ROOT)/lib/pnacl/Release -lppapi -lppapi_cpp -lppapi_gles2 -lpthread -lnacl_io
LDFLAGS_DEBUG := -L$(NACL_SDK_ROOT)/lib/pnacl/Debug -lppapi -lppapi_cpp -lppapi_gles2 -lpthread -lnacl_io

#-std=c++11 
CXXFLAGS := -DNACL -std=gnu++11 -stdlib=libc++ -I$(NACL_SDK_ROOT)/include -I$(NACL_SDK_ROOT)/include/pnacl -I.. $(INCLUDES)
CFLAGS := -DNACL -I$(NACL_SDK_ROOT)/include -I.. $(INCLUDES)

###################################################################################
LIBRARY_FLAGS := $(addprefix -l,$(LIBRARY_NAMES))
LD_LIBRARY_FLAGS := -L$(TARGETDIR) $(LIBRARY_FLAGS)

SOURCES_CC := $(wildcard *.cpp) 
TMP_OBJECTS_CC := $(patsubst %.cpp,%.bc,$(SOURCES_CC))
OBJECTS_CC := $(addprefix $(TARGETDIR)/,$(TMP_OBJECTS_CC))

SOURCES_C := $(wildcard *.c) 
TMP_OBJECTS_C := $(patsubst %.c,%.bc,$(SOURCES_C))
OBJECTS_C := $(addprefix $(TARGETDIR)/,$(TMP_OBJECTS_C))

OBJECTS := $(OBJECTS_CC) $(OBJECTS_C)

ifeq ($(IS_LIBRARY),true)
TARGET := $(TARGETDIR)/lib$(TARGET_NAME).$(TARGET_EXTENSION)
else
TARGET := $(TARGETDIR)/$(TARGET_NAME).$(TARGET_EXTENSION)
endif
TARGET_UNSTRIPPED := $(TARGETDIR)/$(TARGET_NAME)_unstripped.$(TARGET_EXTENSION)
###################################################################################

# Declare the ALL target first, to make the 'all' target the default build
all: $(TARGET)

###################################################################################
# Compile phase
###################################################################################

ifeq ($(CONFIG),Release)
$(TARGETDIR)/%.bc: %.cpp %.h
	$(TOOLCHAIN_CXX) $< $(CXXFLAGS) -c -o $@ -O2  

$(TARGETDIR)/%.bc: %.c %.h
	$(TOOLCHAIN_CXX) $< $(CFLAGS) -c -o $@ -O2  

$(TARGETDIR)/%.bc: %.cpp $(SHADERS)
	$(TOOLCHAIN_CXX) $< $(CXXFLAGS) -c -o $@ -O2  
	
$(TARGETDIR)/%.bc: %.cpp
	$(TOOLCHAIN_CXX) $< $(CXXFLAGS) -c -o $@ -O2  

$(TARGETDIR)/%.bc: %.c
	$(TOOLCHAIN_CXX) $< $(CFLAGS) -c -o $@ -O2  
else
$(TARGETDIR)/%.bc: %.cpp %.h
	$(TOOLCHAIN_CXX) $< $(CXXFLAGS) -c -o $@ -g -O0
	
$(TARGETDIR)/%.bc: %.c %.h
	$(TOOLCHAIN_CXX) $< $(CFLAGS) -c -o $@ -g -O0

$(TARGETDIR)/%.bc: %.cpp $(SHADERS)
	$(TOOLCHAIN_CXX) $< $(CXXFLAGS) -c -o $@ -g -O0
	
$(TARGETDIR)/%.bc: %.cpp
	$(TOOLCHAIN_CXX) $< $(CXXFLAGS) -c -o $@ -g -O0
	
$(TARGETDIR)/%.bc: %.c
	$(TOOLCHAIN_CXX) $< $(CFLAGS) -c -o $@ -g -O0

endif	

#############################################################	
# rules that force the creation of the directory $(TARGETDIR)
#############################################################	
$(OBJECTS): | $(TARGETDIR)

$(TARGETDIR): 
	$(MKDIR) $(TARGETDIR)	

#############################################################		

.PHONY: print
print: 
	@echo NACL_SDK_ROOT=$(NACL_SDK_ROOT)
	@echo PNACL_TC_PATH=$(PNACL_TC_PATH)
	@echo TOOLCHAIN_CXX=$(TOOLCHAIN_CXX)
	@echo TARGETDIR=$(TARGETDIR)
	@echo TARGET=$(TARGET)
	@echo OBJECTS_CC=$(OBJECTS_CC)
	@echo OBJECTS_C$(OBJECTS_C)
	@echo OBJECTS=$(OBJECTS)
	@echo LIBRARIES=$(LIBRARIES)
	@echo LD_LIBRARY_FLAGS=$(LD_LIBRARY_FLAGS)
	@echo LDFLAGS_RELEASE=$(LDFLAGS_RELEASE)
	@echo SOURCES_CC=$(SOURCES_CC)
	@echo SOURCES_C=$(SOURCES_C)
	@echo COPY_DATA_TARGET=$(COPY_DATA_TARGET)
	@echo TARGETDIR1=$(TARGETDIR1)
	$(MAKE) -C web-data print

#############################################################	
# force entering into data folders
#############################################################	
# define DATA_FOLDER_RULE_TEMPLATE
# DATA_FOLDERS += $(TARGETDIR)/$(1)
# .PHONY: $(TARGETDIR)/$(1)
# $(TARGETDIR)/$(1): force_non_subsystems$(1)

# .PHONY:	force_non_subsystems$(1)
# force_non_subsystems$(1):
# 	+$(MAKE) -C $(1)
# endef

# COPY_DATA_TARGET:
# 	$(foreach dir,$(DATA_FOLDER_NAMES),$(eval $(call DATA_FOLDER_RULE_TEMPLATE,$(dir))))

ifeq ($(IS_LIBRARY),true)
#############################################################
# library creation
#############################################################
$(TARGET): $(OBJECTS)
	@echo "******************************************************************************************"
	@echo "Creating library $(TARGET)"
	@echo "******************************************************************************************"
	$(TOOLCHAIN_AR) cr $@ $^
	@echo ""
else
#############################################################
# executable creation & finalize phase
#############################################################
$(TARGET): $(TARGET_UNSTRIPPED) 
	@echo "******************************************************************************************"
	@echo "Finalizing executable $(TARGET)"
	@echo "******************************************************************************************"
	$(TOOLCHAIN_FINALIZE) -o $@ $<
	@echo ""
endif

ifeq ($(CONFIG),Release)
$(TARGET_UNSTRIPPED): $(LIBRARIES) $(OBJECTS) 
	@echo "******************************************************************************************"
	@echo "Creating RELEASE executable $(TARGET)"
	@echo "******************************************************************************************"
	$(TOOLCHAIN_CXX) $(OBJECTS) -o $@  $(LD_LIBRARY_FLAGS) $(LDFLAGS_RELEASE)
	@echo ""
else
$(TARGET_UNSTRIPPED): $(LIBRARIES) $(OBJECTS)
	@echo "******************************************************************************************"
	@echo "Creating DEBUG executable $(TARGET)"
	@echo "******************************************************************************************"
	$(TOOLCHAIN_CXX) $(OBJECTS) -o $@ $(LD_LIBRARY_FLAGS) $(LDFLAGS_DEBUG)
	@echo ""
endif
