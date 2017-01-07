if [ ! -n "$1" ]; then
	echo "Enter SD card device path"
	exit 0
fi

SDCARD=$1
sudo umount $SDCARD

POKY_FOLDER="$HOME/poky"
export MACHINE=beaglebone

echo "1. Partitioning and formatting ${SDCARD}..."

sudo fdisk -lu $SDCARD
sudo mkfs.vfat -S 512 -n "boot" ${SDCARD}1
sudo mke2fs -j -L "root" ${SDCARD}2

echo "2. Installing the boot loaders..."

IMAGES_PATH=$POKY_FOLDER/build-${MACHINE}/tmp/deploy/images/$MACHINE

sudo cp ${IMAGES_PATH}/MLO-$MACHINE /media/$USER/boot/MLO
sudo cp ${IMAGES_PATH}/u-boot-${MACHINE}.img /media/$USER/boot/u-boot.img
sync

echo "3. Installing the root filesystem..."

sudo tar x -C /media/$USER/root -f ${IMAGES_PATH}/nsg-dev-${MACHINE}.tar.bz2 
sync

umount $SDCARD
