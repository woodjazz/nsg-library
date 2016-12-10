isEmpty(PROJECT_PWD) {
    error(PROJECT_PWD has not been set)
}

SOURCE_ROOT_PWD = $$PWD/..
TARGET_PWD = $$OUT_PWD

android {
    DEFINES += IS_TARGET_ANDROID ANDROID GLES2
    QMAKE_CXXFLAGS += -std=gnu++11 -pthread
    HEADERS += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.h
    SOURCES += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.c
    INCLUDEPATH += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue
    LIBS += -ldl -lGLESv2 -llog -landroid -lEGL
}

osx:!android {
    DEFINES += IS_TARGET_OSX
    DEFINES += IS_TARGET_APPLE
    QMAKE_CXXFLAGS += -x objective-c++ -std=c++11 -stdlib=libc++
    QMAKE_LFLAGS += -F/System/Library/Frameworks/
    LIBS += -framework AudioUnit -framework Carbon -framework Cocoa -framework CoreAudio -framework ForceFeedback -framework IOKit -framework OpenGL -framework CoreServices
    BLENDER_EXECUTABLE = $$(BLENDER_BIN)/blender.app/Contents/MacOS/blender

}

linux:!android {
    DEFINES += IS_TARGET_LINUX
    BLENDER_EXECUTABLE = $$(BLENDER_BIN)/blender
}

win32:!android {
    DEFINES += IS_TARGET_WINDOWS
    BLENDER_EXECUTABLE = $$(BLENDER_BIN)/blender.exe
}

OTHER_FILES += $$PROJECT_PWD/data/*
OTHER_FILES += $$PROJECT_PWD/art/*

QMAKE_CXXFLAGS_DEBUG += -D_DEBUG
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG
QMAKE_LFLAGS_RELEASE -= -O1
QMAKE_LFLAGS_RELEASE += -O3
QMAKE_LFLAGS_DEBUG += -g

TARGET = $$basename(PROJECT_PWD)
EXPORT_SCRIPT = $$SOURCE_ROOT_PWD/tools/blenderexport/Export.py

defineTest(exportBlend) {
    exists($$BLENDER_EXECUTABLE) {
        file = $$1
        fileFullPath = $$PROJECT_PWD/$$file
        name = $$basename(file)
        target = $$PROJECT_PWD/$$2/$$basename(fileFullPath).xml
        eval($${name}.target = $$target)
        export($${name}.target)
        eval($${name}.commands = $$BLENDER_EXECUTABLE $$PROJECT_PWD/$${file}.blend --background --python $$EXPORT_SCRIPT -- $$PROJECT_PWD/$$2)
        export($${name}.commands)
        QMAKE_EXTRA_TARGETS += $${name}
        PRE_TARGETDEPS += $$target
        osx:!android {
            cptarget = $${name}1
            eval($${cptarget}.commands = cp -R $$PROJECT_PWD/data $$OUT_PWD/$${TARGET}.app/Contents/Resources)
            export($${cptarget}.commands)
            eval($${cptarget}.depends = $${name})
            export($${cptarget}.depends)
            QMAKE_EXTRA_TARGETS += $${cptarget}
            PRE_TARGETDEPS += $${cptarget}
        }
        export(QMAKE_EXTRA_TARGETS)
        export(PRE_TARGETDEPS)
    }
}

defineTest(copyData) {
    osx:!android {
        copyData.commands = cp -R $$PROJECT_PWD/data $$OUT_PWD/$${TARGET}.app/Contents/Resources
        export(copyData.commands)
        QMAKE_EXTRA_TARGETS += copyData
        PRE_TARGETDEPS += copyData
    }
    export(QMAKE_EXTRA_TARGETS)
    export(PRE_TARGETDEPS)
}

defineTest(setupSample) {
    TEMPLATE = app
    export(TEMPLATE)
    HEADERS += $$files($$PROJECT_PWD/*.h, true)
    export(HEADERS)
    SOURCES += $$files($$PROJECT_PWD/*.cpp, true)
    export(SOURCES)
    INCLUDEPATH += $$files($$SOURCE_ROOT_PWD/NSG/*, false)
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/pugixml/src
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/imgui
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/bullet/src
    export(INCLUDEPATH)
    LIBS += -L../../NSG -lNSG
    LIBS += -L../../dependencies/jpeg -ljpeg
    LIBS += -L../../dependencies/libb64 -llibb64
    LIBS += -L../../dependencies/LZ4 -lLZ4
    LIBS += -L../../externals -lexternals
    export(LIBS)
}

defineTest(setupTest) {
    TEMPLATE = app
    export(TEMPLATE)
    CONFIG += testcase
    export(CONFIG)
    HEADERS += $$files($$PROJECT_PWD/*.h, true)
    export(HEADERS)
    SOURCES += $$files($$PROJECT_PWD/*.cpp, true)
    export(SOURCES)
    INCLUDEPATH += $$files($$SOURCE_ROOT_PWD/NSG/*, false)
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/pugixml/src
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/imgui
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/bullet/src
    export(INCLUDEPATH)
    LIBS += -L../../NSG -lNSG
    LIBS += -L../../dependencies/jpeg -ljpeg
    LIBS += -L../../dependencies/libb64 -llibb64
    LIBS += -L../../dependencies/LZ4 -lLZ4
    LIBS += -L../../externals -lexternals
    export(LIBS)
}

defineTest(setupTool) {
    TEMPLATE = app
    export(TEMPLATE)
    HEADERS += $$files($$PROJECT_PWD/*.h, true)
    export(HEADERS)
    SOURCES += $$files($$PROJECT_PWD/*.cpp, true)
    export(SOURCES)
    INCLUDEPATH += $$PROJECT_PWD/../common
    INCLUDEPATH += $$files($$SOURCE_ROOT_PWD/NSG/*, false)
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/pugixml/src
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/imgui
    INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/bullet/src
    INCLUDEPATH += $$SOURCE_ROOT_PWD/dependencies/tclap/include
    export(INCLUDEPATH)
    LIBS += -L../../NSG -lNSG
    LIBS += -L../../dependencies/jpeg -ljpeg
    LIBS += -L../../dependencies/libb64 -llibb64
    LIBS += -L../../dependencies/LZ4 -lLZ4
    LIBS += -L../../externals -lexternals
    LIBS += -L../../tools/common -lcommon
    export(LIBS)
}


