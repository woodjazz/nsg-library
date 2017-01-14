#!/bin/bash

# git://code.qt.io/yocto/meta-boot2qt.git

printUsage ()
{
    echo "Usage: $0 [--image <bb file>] [--machine <machine>]"
}

while test -n "$1"; do
  case "$1" in
    "--image")
      IMAGE_=$1
      ;;
    "--machine")
      shift
      MACHINE_=$1
      ;;
    *)
      printUsage
      exit 0
      ;;
  esac
  shift
done

if [ -z "${MACHINE_}" ]; then
	MACHINE_=qemux86
fi

if [ -z "${IMAGE_}" ]; then
	IMAGE_=core-image-full-cmdline 
fi

SOURCE_FOLDER="$PWD"
POKY_FOLDER="$HOME/poky"
export MACHINE=${MACHINE_}

if [ ! -d "$POKY_FOLDER" ]; then
	git clone git://git.yoctoproject.org/poky $POKY_FOLDER
fi

if [ ! -d "$POKY_FOLDER/meta-qt5" ]; then
	git clone git://code.qt.io/yocto/meta-qt5.git $POKY_FOLDER/meta-qt5
fi

cd $POKY_FOLDER
git checkout -b morty origin/morty

source ./oe-init-build-env build-$MACHINE

cp $SOURCE_FOLDER/conf/bblayers.conf $POKY_FOLDER/build-$MACHINE/conf
cp $SOURCE_FOLDER/conf/local.conf $POKY_FOLDER/build-$MACHINE/conf

echo -ne '\n\n'Making image ${IMAGE_}...'\n'
bitbake ${IMAGE_}
echo -ne '\n\n'Populating SDK for ${IMAGE_}...'\n'
bitbake ${IMAGE_} -c populate_sdk
sudo rm -rf "/opt/poky/${MACHINE_}" 
printf "/opt/poky/${MACHINE_}" | ${BUILDDIR}/tmp/deploy/sdk/*${IMAGE_}*.sh

echo -ne '\n\nMaking meta-toolchain-qt5...\n'
bitbake meta-toolchain-qt5
sudo rm -rf /opt/poky/qt5
printf "/opt/poky/qt5" | ${BUILDDIR}/tmp/deploy/sdk/*meta-toolchain-qt5*.sh

#runqemu qemux86
#ssh root@192.168.7.2
#scp xxxx root@192.168.7.2:~
#sshpass -p abcd scp * root@192.168.7.2:~

# remote debugging (target)
#gdbserver HOST:PORT PROG

# debugging on host
# gdb 
# set auto-load safe-path /
# set sysroot $SDKTARGETSYSROOT
# target remote ip:port
# break main
# continue

#lsblk -d
#dd bs=4k if=b2qt-embedded-qt5-image-beaglebone.img of=/dev/sdf status=progress; sync
#pv -p b2qt-embedded-qt5-image-beaglebone.img | sudo dd bs=4k of=/dev/sdf
#dd if=b2qt-embedded-qt5-image-beaglebone.img | pv -p | sudo dd of=/dev/sdf
#find . -print | grep zlib
#grep -r IMAGE_TYPES .
#sudo dd bs=4k if=nsg-dev-beaglebone.wic of=/dev/sdf status=progress; sync
