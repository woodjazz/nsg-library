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
SOURCE_HTMLS := $(wildcard *.html)
TARGET_HTMLS := $(addprefix $(TARGETDIR)/,$(SOURCE_HTMLS))
SOURCE_JAVASCRIPT := $(wildcard *.js)
TARGET_JAVASCRIPT := $(addprefix $(TARGETDIR)/,$(SOURCE_JAVASCRIPT))

SOURCE_NMF := $(wildcard *.nmf)
TARGET_NMF := $(addprefix $(TARGETDIR)/,$(SOURCE_NMF))

SOURCE_JSON := $(wildcard *.json)
TARGET_JSON := $(addprefix $(TARGETDIR)/,$(SOURCE_JSON))

PNG_ASSETS := $(wildcard *.png)
TARGET_PNG_ASSETS := $(addprefix $(TARGETDIR)/,$(PNG_ASSETS))
###################################################################################

COPY_DATA_TARGET: $(TARGET_JAVASCRIPT) $(TARGET_HTMLS) $(TARGET_NMF) $(TARGET_PNG_ASSETS) $(TARGET_JSON)
DATA_TARGETS := $(TARGET_JAVASCRIPT) $(TARGET_HTMLS) $(TARGET_NMF) $(TARGET_PNG_ASSETS) $(TARGET_JSON)
include ../common.mk

#############################################################
# Copy needed files (by HTTP server) to $(TARGETDIR)
#############################################################
$(TARGETDIR)/%.html: %.html
	$(CP) $< $@
	
$(TARGETDIR)/%.nmf: %.nmf
	$(CP) $< $@

$(TARGETDIR)/%.js: %.js
	$(CP) $< $@

$(TARGETDIR)/%.png: %.png
	$(CP) $< $@
 
$(TARGETDIR)/%.json: %.json
	$(CP) $< $@

#############################################################		
.PHONY: print
print: 
	@echo TARGETDIR=$(TARGETDIR)
