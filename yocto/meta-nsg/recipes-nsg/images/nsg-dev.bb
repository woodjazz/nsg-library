SUMMARY = "A small image for my project."

IMAGE_INSTALL = "packagegroup-core-boot ${ROOTFS_PKGMANAGE_BOOTSTRAP} ${CORE_IMAGE_EXTRA_INSTALL}"

IMAGE_LINGUAS = " "

LICENSE = "MIT"

inherit core-image

IMAGE_ROOTFS_SIZE ?= "8192"
IMAGE_ROOTFS_EXTRA_SPACE = "1000000"
IMAGE_ROOTFS_EXTRA_SPACE_append = "${@bb.utils.contains("DISTRO_FEATURES", "systemd", " + 4096", "" ,d)}"

MACHINE_FEATURES += "keyboard screen usbgadget usbhost"
DISTRO_FEATURES += "opengl x11"
IMAGE_FEATURES += "debug-tweaks dbg-pkgs dev-pkgs staticdev-pkgs hwcodecs tools-debug ssh-server-openssh x11-base x11"
IMAGE_INSTALL += "keymaps libx11 libxau libxcalibrate libxcb libxcomposite libxcursor libxdamage libxdmcp libxext libxfixes libxfont libxft libxi libxinerama libxkbcommon libxkbfile libxmu libxpm libxrandr libxrender libxres libxscrnsaver libxshmfence libxt libxtst libxv libxvmc libxxf86dga libxxf86misc libxxf86vm gdbserver packagegroup-core-x11-xserver libgcc libgcc-dev libstdc++ libstdc++-dev libstdc++-staticdev packagegroup-core-buildessential binutils"
