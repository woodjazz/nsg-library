message(Qt MAKESPEC: $$QMAKESPEC)
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Documentation: $$[QT_INSTALL_DOCS])
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Plugins: $$[QT_INSTALL_PLUGINS])
message(Data files: $$[QT_INSTALL_DATA])
message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
message(Settings: $$[QT_INSTALL_CONFIGURATION])
message(Examples: $$[QT_INSTALL_EXAMPLES])

android {
    message("Detected Android platform")
}

osx:!android {
    message("Detected OSX platform")
}

linux:!android {
    message("Detected Linux platform")
}

win32:!android {
    message("Detected Windows platform")
}

TEMPLATE = subdirs
CONFIG += ordered
OTHER_FILES = .qmake.conf qmake/* *
SUBDIRS = NSG\
externals\
dependencies\
tools\
tests\
samples\



