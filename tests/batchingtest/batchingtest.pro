include(../../qmake/common.pro)
TEMPLATE = app
HEADERS = $$files(*.h, true)
SOURCES = $$files(*.cpp, true)
INCLUDEPATH += $$setupIncludes()
LIBS += $$setupLibs()


