PROJECT_PWD = $$PWD
include(../../qmake/common.pri)
args = -f 32 -x 256 -y 256 -s 32 -e 126
convertTool(art/AnonymousPro.ttf, data, $$args)
setupSample()
