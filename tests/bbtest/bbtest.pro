include(../../qmake/common.pri)
TEMPLATE = app
CONFIG += testcase
HEADERS = $$files(*.h, true)
SOURCES = $$files(*.cpp, true)
LIBS += $$setupLibs()

