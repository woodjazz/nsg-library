TEMPLATE = lib
CONFIG += staticlib
HEADERS += $$files(*.h, true)
SOURCES += $$files(*.cpp, true)
SOURCES += $$files(*.c, true)
INCLUDEPATH += $$files($$SOURCE_ROOT_PWD/NSG/*, false)
INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/pugixml/src
INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/imgui
INCLUDEPATH += $$SOURCE_ROOT_PWD/externals/bullet/src
INCLUDEPATH += \
../externals/bullet/src/LinearMath\
../externals/bullet/src/BulletCollision\
../externals/bullet/src/BulletDynamics\
../externals/bullet/src/BulletSoftBody\
../externals/stb\
../dependencies/jpeg\
../dependencies/libb64/include\
../dependencies/LZ4\

android {
    HEADERS += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.h
    SOURCES += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.c
    INCLUDEPATH += $$(ANDROID_NDK_ROOT)/sources/android/native_app_glue
}
