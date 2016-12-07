include(../qmake/common.pro)
TEMPLATE = lib
CONFIG = staticlib
HEADERS = $$files(*.h, true)
SOURCES = $$files(*.cpp, true)
SOURCES += $$files(*.c, true)

INCLUDEPATH += $$files(*, true)
INCLUDEPATH += ../externals/imgui\
../externals/pugixml/src\
../externals/bullet/src\
../externals/bullet/src/LinearMath\
../externals/bullet/src/BulletCollision\
../externals/bullet/src/BulletDynamics\
../externals/bullet/src/BulletSoftBody\
../externals/stb\
../dependencies/jpeg\
../dependencies/libb64/include\
../dependencies/LZ4\





