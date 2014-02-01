include ../common.mk

$(TARGET): $(OBJECTS)
	$(TOOLCHAIN_AR) cr $@ $^

