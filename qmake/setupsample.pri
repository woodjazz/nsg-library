include(common.pri)
TEMPLATE = app
HEADERS += $$files($$PROJECT_PWD/*.h, true)
SOURCES += $$files($$PROJECT_PWD/*.cpp, true)
INCLUDEPATH += $$files($$ROOT_PWD/NSG/*, false)
INCLUDEPATH += $$ROOT_PWD/externals/pugixml/src
INCLUDEPATH += $$ROOT_PWD/externals/imgui
INCLUDEPATH += $$ROOT_PWD/externals/bullet/src
LIBS += -L../../NSG -lNSG
LIBS += -L../../dependencies/jpeg -ljpeg
LIBS += -L../../dependencies/libb64 -llibb64
LIBS += -L../../dependencies/LZ4 -lLZ4
LIBS += -L../../externals -lexternals

