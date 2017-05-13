#!/bin/sh
set -e
case "${TRAVIS_OS_NAME}" in
  linux)
	pwd && mkdir -p debug && cd debug && cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" && make && make test
	cd ..
	mkdir -p release && cd release && cmake .. -G "Unix Makefiles" && make && make test
	cd ..
	#./cmake_ems.sh emsbuild && cd../emsbuild && make
    ;;
  osx)
	./cmake_osx.sh && cd build_ && xcodebuild -target ALL_BUILD #&& xcodebuild -target RUN_TESTS
    ;;
esac
