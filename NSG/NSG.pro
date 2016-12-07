include(../qmake/common.pri)
TEMPLATE = lib
CONFIG = staticlib
HEADERS = $$files(*.h, true)
SOURCES = $$files(*.cpp, true)
SOURCES += $$files(*.c, true)

INCLUDEPATH += \
../externals/bullet/src/LinearMath\
../externals/bullet/src/BulletCollision\
../externals/bullet/src/BulletDynamics\
../externals/bullet/src/BulletSoftBody\
../externals/stb\
../dependencies/jpeg\
../dependencies/libb64/include\
../dependencies/LZ4\
