#!/bin/sh
set -e
case "${TRAVIS_OS_NAME}" in
  linux)
    # INSTALL gcc 4.8.1 AND g++ 4.8.1
    travis_retry sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test\
    travis_retry sudo apt-get update -qq\
    travis_retry sudo apt-get install -qq gcc-4.8 g++-4.8\
    travis_retry sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 90\
    # GET LINUX BUILD DEPENDENCIES
    travis_retry sudo apt-get install -y libglu1-mesa-dev\
    travis_retry sudo apt-get install -q -y --no-install-recommends libasound2-dev
    ;;
  osx)
    ;;
esac
