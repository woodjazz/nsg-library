POKY_FOLDER="$HOME/poky"
rm -rf $POKY_FOLDER/meta-nsg
cp -r meta-nsg $POKY_FOLDER
cd $POKY_FOLDER
source ./oe-init-build-env
# https://github.com/meta-qt5/meta-qt5
bitbake meta-toolchain-qt5
./tmp/deploy/sdk/poky-glibc-x86_64-meta-toolchain-qt5-i586-toolchain-2.2.sh
