#!/bin/sh
set -e
case "${TRAVIS_OS_NAME}" in
  linux)
  	./cmake_linux.sh linuxbuild && cd ../linuxbuild && make && make test
    ;;
  osx)
	./cmake_osx.sh osxbuild && cd ../osxbuild && xcodebuild -target ALL_BUILD && xcodebuild -target RUN_TESTS
    ;;
esac
