SUMMARY = "A small image for my project."

#require recipes-core/images/core-image-minimal.bb
require recipes-sato/images/core-image-sato.bb

inherit extrausers
EXTRA_USERS_PARAMS = "usermod -P abcd root;"

IMAGE_ROOTFS_EXTRA_SPACE = "1000000"

IMAGE_FEATURES += "\
package-management \
ssh-server-dropbear \
tools-debug \
debug-tweaks \
hwcodecs \
"

IMAGE_INSTALL += "\
	libxcursor \
	packagegroup-qt5-qtcreator-debug \
	freetype \
	fontconfig \
	liberation-fonts \
	ttf-bitstream-vera \
	mesa \
    qt3d \
    qtbase \
    qtcanvas3d \
    qtcharts \
    qtconnectivity \
    qtdatavis3d \
    qtdeclarative \
    qtdeclarative-render2d \
    qtdeclarative-tools \
    qtgraphicaleffects \
    qtimageformats \
    qtlocation \
    qtmultimedia \
    qtquickcontrols \
    qtquickcontrols2 \
    qtsensors \
    qtserialbus \
    qtserialport \
    qtsvg \
    qttools \
    qttranslations-qt \
    qttranslations-qtbase \
    qttranslations-qtdeclarative \
    qttranslations-qtconnectivity \
    qttranslations-qtlocation \
    qttranslations-qtmultimedia \
    qttranslations-qtquickcontrols \
    qttranslations-qtserialport \
    qttranslations-qtwebsockets \
    qttranslations-qtxmlpatterns \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'qtwayland', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'webengine', 'qtwebengine qttranslations-qtwebengine', '', d)} \
    qtwebsockets \
    qtwebchannel \
    qtxmlpatterns \
    qtvirtualkeyboard \
    ldd \
    binutils \
    binutils-symlinks \
    i2c-tools \
    perf \
    connman-client \
    iproute2 \
    rsync \
    ${@bb.utils.contains("DISTRO_FEATURES", "systemd", "systemd-analyze", "", d)} \    
    "