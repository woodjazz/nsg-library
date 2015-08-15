#!/bin/sh
set -e
case "${TRAVIS_OS_NAME}" in
  linux)
    # SIMULATE GUI (DISPLAY)
    sh -e /etc/init.d/xvfb start
    # INSTALL gcc 4.8.1 AND g++ 4.8.1
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
    sudo apt-get update -qq
    sudo apt-get install -qq gcc-4.8 g++-4.8
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 90
    # GET LINUX BUILD DEPENDENCIES
    sudo apt-get install -y libglu1-mesa-dev
    sudo apt-get install -q -y --no-install-recommends libasound2-dev
    ;;
  osx)
    wget https://dl.dropboxusercontent.com/u/203889738/gcc/XQuartz.pkg
    sudo installer -pkg ./XQuartz.pkg -target /
    # SIMULATE GUI (DISPLAY)
    sh -e /opt/X11/bin/Xvfb start
    ;;
esac
