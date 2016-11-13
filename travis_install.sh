#!/bin/sh
set -e
case "${TRAVIS_OS_NAME}" in
  linux)
    # SIMULATE GUI (DISPLAY)
    sh -e /etc/init.d/xvfb start
    # INSTALL gcc 4.8.1 AND g++ 4.8.1
    # sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
    # sudo apt-get update -qq
    # sudo apt-get install -qq gcc-4.8 g++-4.8
    # sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 90
    # GET LINUX BUILD DEPENDENCIES
    # sudo apt-get install -y libglu1-mesa-dev
    # sudo apt-get install -q -y --no-install-recommends libasound2-dev
    sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu $(lsb_release -sc) main universe restricted multiverse"
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
    sudo apt-get update -qq
    # GET LINUX BUILD DEPENDENCIES
    sudo apt-get install -q -y xorg-dev
    sudo apt-get install libglu1-mesa-dev
    #sudo apt-get install -q -y --no-install-recommends libasound2-dev
    sudo apt-get install -q -y libc++-dev #needed for clang
    # EMSCRIPTEN
    wget https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-portable.tar.gz
    tar xvf emsdk-portable.tar.gz
    cd emsdk_po*
    ./emsdk update
    ./emsdk install latest
    ./emsdk activate latest
    source ./emsdk_env.sh
    ;;
  osx)
    #brew install Caskroom/cask/xquartz
    #wget https://dl.dropboxusercontent.com/u/203889738/gcc/XQuartz.pkg
    #sudo installer -pkg ./XQuartz.pkg -target /
    # SIMULATE GUI (DISPLAY)
    Xvfb $DISPLAY &
    ;;
esac
