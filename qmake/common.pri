TOP_PWD = $$PWD/..
osx {
    message("Detected OSX platform")
    DEFINES += IS_TARGET_OSX
    DEFINES += IS_TARGET_APPLE
    QMAKE_CXXFLAGS += -Wno-deprecated-register -x objective-c++ -std=c++11 -stdlib=libc++
    QMAKE_LFLAGS += -F/System/Library/Frameworks/
    LIBS += -framework AudioUnit -framework Carbon -framework Cocoa -framework CoreAudio -framework ForceFeedback -framework IOKit -framework OpenGL -framework CoreServices
}

linux {
    message("Detected Linux platform")
    DEFINES += IS_TARGET_LINUX
}

win32 {
    message("Detected Windows platform")
    DEFINES += IS_TARGET_WINDOWS
}

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

INCLUDEPATH += $$files($$TOP_PWD/NSG/*, false)
INCLUDEPATH += $$TOP_PWD/externals/pugixml/src
INCLUDEPATH += $$TOP_PWD/externals/imgui
INCLUDEPATH += $$TOP_PWD/externals/bullet/src

#LIBS += -L$$TOP_PWD/NSG -lNSG
#LIBS += -L$$TOP_PWD/dependencies/jpeg -ljpeg
#LIBS += -L$$TOP_PWD/dependencies/libb64 -llibb64
#LIBS += -L$$TOP_PWD/dependencies/LZ4 -lLZ4
#LIBS += -L$$TOP_PWD/externals -lexternals

defineReplace(setupIncludes) {
    includes = $$files(../../NSG/*, false)
    includes += ../../externals/pugixml/src
    includes += ../../externals/imgui
    includes += ../../externals/bullet/src
    return ($$includes)
}

defineReplace(setupLibs) {
    libs = -L../../NSG -lNSG
    libs += -L../../dependencies/jpeg -ljpeg
    libs += -L../../dependencies/libb64 -llibb64
    libs += -L../../dependencies/LZ4 -lLZ4
    libs += -L../../externals -lexternals
    return ($$libs)
}

#message($$LIBS)


