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
include common.mk

#############################################################	
# generate static libraries
#############################################################	
define SUBSYSTEM_RULE_TEMPLATE
LIBRARIES += $(TARGETDIR)/lib$(1).a
.PHONY: $(TARGETDIR)/$(1).a
$(TARGETDIR)/lib$(1).a: force_subsystems$(1)

.PHONY:	force_subsystems$(1)
force_subsystems$(1):
	+$(MAKE) -C $(1) TARGET_NAME=lib$(1)
endef

$(foreach dir,$(LIBRARY_NAMES),$(eval $(call SUBSYSTEM_RULE_TEMPLATE,$(dir))))

#############################################################	
# force entering into data folders
#############################################################	
define DATA_FOLDER_RULE_TEMPLATE
DATA_FOLDERS += $(TARGETDIR)/$(1)
.PHONY: $(TARGETDIR)/$(1)
$(TARGETDIR)/$(1): force_non_subsystems$(1)

.PHONY:	force_non_subsystems$(1)
force_non_subsystems$(1):
	+$(MAKE) -C $(1)
endef

$(foreach dir,$(DATA_FOLDER_NAMES),$(eval $(call DATA_FOLDER_RULE_TEMPLATE,$(dir))))

#############################################################
# Finalize phase
#############################################################
$(TARGET): $(TARGET_UNSTRIPPED)
	$(TOOLCHAIN_FINALIZE) -o $@ $<
	
#############################################################
# Link phase
#############################################################
ifeq ($(CONFIG),Release)
$(TARGET_UNSTRIPPED): $(DATA_FOLDERS) $(LIBRARIES) $(OBJECTS) 
	$(TOOLCHAIN_CXX) $(OBJECTS) -o $@  $(LD_LIBRARY_FLAGS) $(LDFLAGS_RELEASE)
else
$(TARGET_UNSTRIPPED): $(DATA_FOLDERS) $(LIBRARIES) $(OBJECTS)
	$(TOOLCHAIN_CXX) $(OBJECTS) -o $@ $(LD_LIBRARY_FLAGS) $(LDFLAGS_DEBUG)
endif
