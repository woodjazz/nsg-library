POKY_FOLDER="$HOME/poky"
rm -rf $POKY_FOLDER/meta-nsg
cp -r meta-nsg $POKY_FOLDER
cd $POKY_FOLDER
source ./oe-init-build-env
bitbake nsg-dev
bitbake nsg-dev -c populate_sdk
./tmp/deploy/sdk/poky-glibc-x86_64-nsg-dev-i586-toolchain-2.2.sh
#runqemu qemux86
#ssh root@192.168.7.2
#scp xxxx root@192.168.7.2:~
