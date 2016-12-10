PROJECT_PWD = $$PWD
include(../../qmake/common.pri)
TEMPLATE = lib
CONFIG += staticlib
HEADERS += $$files(*.h, true)
SOURCES += $$files(*.cpp, true)
SOURCES += $$files(*.c, true)
INCLUDEPATH += $$files($$SOURCE_ROOT_PWD/NSG/*, false)
INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/pugixml/src
INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/imgui
INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/bullet/src
INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/stb
