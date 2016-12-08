ROOT_PWD = $$PWD/..

osx {
    #message("Detected OSX platform")
    DEFINES += IS_TARGET_OSX
    DEFINES += IS_TARGET_APPLE
    QMAKE_CXXFLAGS += -x objective-c++ -std=c++11 -stdlib=libc++
    QMAKE_LFLAGS += -F/System/Library/Frameworks/
    LIBS += -framework AudioUnit -framework Carbon -framework Cocoa -framework CoreAudio -framework ForceFeedback -framework IOKit -framework OpenGL -framework CoreServices
    OTHER_FILES = $$PROJECT_PWD/data/*
    DATA.files = $$files($$PROJECT_PWD/data/*, true)
    DATA.path = Contents/Resources/data
    QMAKE_BUNDLE_DATA += DATA
}

linux {
    message("Detected Linux platform")
    DEFINES += IS_TARGET_LINUX
}

win32 {
    message("Detected Windows platform")
    DEFINES += IS_TARGET_WINDOWS
}

QMAKE_CXXFLAGS_DEBUG += -D_DEBUG
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG
QMAKE_LFLAGS_RELEASE -= -O1
QMAKE_LFLAGS_RELEASE += -O3
QMAKE_LFLAGS_DEBUG += -g

TARGET = $$basename(PROJECT_PWD)


