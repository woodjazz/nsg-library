osx {
    message("Detected OSX platform")
    DEFINES += IS_TARGET_OSX
    DEFINES += IS_TARGET_APPLE
    QMAKE_CXXFLAGS += -Wno-deprecated-register -x objective-c++ -stdlib=libc++
}

linux {
    message("Detected Linux platform")
    DEFINES += IS_TARGET_LINUX
}

win32 {
    message("Detected Windows platform")
    DEFINES += IS_TARGET_WINDOWS
}

TEMPLATE = lib
CONFIG += c++11 staticlib
HEADERS = $$files(*.h, true)
SOURCES = $$files(*.cpp, true)

INCLUDEPATH += $$files(*, true)
INCLUDEPATH += ../externals/imgui\
../externals/pugixml/src\
../externals/bullet/src/\
../externals/bullet/src/LinearMath\
../externals/bullet/src/BulletCollision\
../externals/bullet/src/BulletDynamics\
../externals/bullet/src/BulletSoftBody




