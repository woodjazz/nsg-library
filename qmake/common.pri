ROOT_PWD = $$PWD/..

osx {
    message("Detected OSX platform")
    DEFINES += IS_TARGET_OSX
    DEFINES += IS_TARGET_APPLE
    QMAKE_CXXFLAGS += -x objective-c++ -std=c++11 -stdlib=libc++
    QMAKE_LFLAGS += -F/System/Library/Frameworks/
    LIBS += -framework AudioUnit -framework Carbon -framework Cocoa -framework CoreAudio -framework ForceFeedback -framework IOKit -framework OpenGL -framework CoreServices
    DATA.files = $$files($$PROJECT_PWD/data/*, true)
    DATA.path = Contents/Resources/data
    QMAKE_BUNDLE_DATA += DATA
}

android {
    message("Detected Android platform")
    DEFINES += IS_TARGET_ANDROID ANDROID GLES2
    QMAKE_CXXFLAGS += -std=gnu++11 -pthread
    HEADERS += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.h
    SOURCES += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.c
    INCLUDEPATH += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue
    LIBS += -ldl -lGLESv2 -llog -landroid -lEGL
}

linux:!android {
    message("Detected Linux platform")
    DEFINES += IS_TARGET_LINUX
}

win32:!android {
    message("Detected Windows platform")
    DEFINES += IS_TARGET_WINDOWS
}

OTHER_FILES = $$PROJECT_PWD/data/*
QMAKE_CXXFLAGS_DEBUG += -D_DEBUG
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG
QMAKE_LFLAGS_RELEASE -= -O1
QMAKE_LFLAGS_RELEASE += -O3
QMAKE_LFLAGS_DEBUG += -g

TARGET = $$basename(PROJECT_PWD)


