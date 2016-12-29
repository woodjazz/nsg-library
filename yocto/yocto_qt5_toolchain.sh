POKY_FOLDER="$HOME/poky"
rm -rf $POKY_FOLDER/meta-nsg
cp -r meta-nsg $POKY_FOLDER
cd $POKY_FOLDER
source ./oe-init-build-env
# https://github.com/meta-qt5/meta-qt5
bitbake meta-toolchain-qt5
