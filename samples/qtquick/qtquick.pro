!android {
    CONFIG += qt
    QT += qml quick
    setupSample()
    RESOURCES += main.qml qtQuick.qrc
    exportBlend(art/duck, data)
}
