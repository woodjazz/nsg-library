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

defineTest(createDir) {
    dir = $$1
    !exists($$dir) {
        mkpath($$dir)
    }
}

defineTest(addSubdirs) {
    TEMPLATE = subdirs
    export(TEMPLATE)
    subdirs = $$files($$PROJECT_PWD/*, false)
    currentProj = $$PROJECT_PWD/$${TARGET}.pro
    for(subdir, subdirs) {
        !equals(currentProj, $$subdir) {
            SUBDIRS += $$basename(subdir)
        }
    }
    #message($$SUBDIRS)
    export (SUBDIRS)
}

defineTest(copyData) {
    dir = $$PROJECT_PWD/data
    createDir($$dir)
    osx {
        copyData.commands = cp -R $$dir $$OUT_PWD/$${TARGET}.app/Contents/Resources
    }
    win32 {
        copyData.commands = xcopy $$dir $$OUT_PWD/$${TARGET}
    }
    linux {
        copyData.commands = cp -R $$dir $$OUT_PWD/$${TARGET}
    }
    export(copyData.commands)
    QMAKE_EXTRA_TARGETS += copyData
    PRE_TARGETDEPS += copyData
    export(QMAKE_EXTRA_TARGETS)
    export(PRE_TARGETDEPS)
}

defineTest(exportBlend) {
    exists($$BLENDER_EXECUTABLE) {
        exportScript = $$SOURCE_ROOT_PWD/tools/blenderexport/Export.py
        file = $$1
        fileFullPath = $$PROJECT_PWD/$$file
        name = $$file
        target = $$PROJECT_PWD/$$2/$$basename(fileFullPath).xml
        outputDir = $$PROJECT_PWD/$$2
        createDir($$outputDir)
        eval($${name}.target = $$target)
        export($${name}.target)
        eval($${name}.commands = $$BLENDER_EXECUTABLE $$PROJECT_PWD/$${file}.blend --background --python $$exportScript -- $$outputDir)
        export($${name}.commands)
        QMAKE_EXTRA_TARGETS += $${name}
        PRE_TARGETDEPS += $$target
        cptarget = $${name}1
        osx {
            eval($${cptarget}.commands = cp -R $$PROJECT_PWD/data $$OUT_PWD/$${TARGET}.app/Contents/Resources)
        } else {
            eval($${cptarget}.commands = cp -R $$PROJECT_PWD/data $$OUT_PWD/$${TARGET})
        }
        export($${cptarget}.commands)
        eval($${cptarget}.depends = $${name})
        export($${cptarget}.depends)
        QMAKE_EXTRA_TARGETS += $${cptarget}
        PRE_TARGETDEPS += $${cptarget}
        export(QMAKE_EXTRA_TARGETS)
        export(PRE_TARGETDEPS)
    }
}

defineTest(convertTool) {
    file = $$PROJECT_PWD/$$1
    outputDir = $$PROJECT_PWD/$$2
    createDir($$outputDir)
    args = $$3
    convertExecutable = $$TARGET_PWD/../../tools/converter/converter.app
    name = $$file
    target = $$outputDir/$$basename(name).xml
    eval($${name}.target = $$target)
    export($${name}.target)
    osx {
        eval($${name}.commands = open $$convertExecutable --args -i $$file -o $$outputDir $$args)
    } else {
        eval($${name}.commands = $$convertExecutable -i $$file -o $$outputDir $$args)
    }
    export($${name}.commands)
    QMAKE_EXTRA_TARGETS += $${name}
    PRE_TARGETDEPS += $$target
    cpname = $${name}1
    osx {
        outputTargetDir = $$OUT_PWD/$${TARGET}.app/Contents/Resources
    } else {
        outputTargetDir = $$OUT_PWD/$${TARGET}
    }
    eval($${cpname}.commands = cp -R $$PROJECT_PWD/data $$outputTargetDir)
    export($${cpname}.commands)
    eval($${cpname}.depends = $${name})
    export($${cpname}.depends)
    QMAKE_EXTRA_TARGETS += $$cpname
    PRE_TARGETDEPS += $$cpname
    export(QMAKE_EXTRA_TARGETS)
    export(PRE_TARGETDEPS)
}


defineTest(setupSample) {
    copyData()
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
    copyData()
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


