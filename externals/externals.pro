TEMPLATE = lib
CONFIG += staticlib
HEADERS = imgui/*.h
SOURCES = $$files(imgui/*.cpp, false)
HEADERS += pugixml/src/*.hpp
SOURCES += $$files(pugixml/src/*.cpp, false)
HEADERS += $$files(bullet/src/LinearMath/*.h, true)
SOURCES += $$files(bullet/src/LinearMath/*.cpp, true)
HEADERS += $$files(bullet/src/BulletCollision/*.h, true)
SOURCES += $$files(bullet/src/BulletCollision/*.cpp, true)
HEADERS += $$files(bullet/src/BulletDynamics/*.h, true)
SOURCES += $$files(bullet/src/BulletDynamics/*.cpp, true)
HEADERS += $$files(bullet/src/BulletSoftBody/*.h, true)
SOURCES += $$files(bullet/src/BulletSoftBody/*.cpp, true)
INCLUDEPATH = bullet/src

