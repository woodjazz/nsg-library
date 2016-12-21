TEMPLATE = lib
CONFIG += staticlib
HEADERS = $$files(include/b64/*.h, false)
SOURCES = src/cdecode.c src/cencode.c
INCLUDEPATH = include
