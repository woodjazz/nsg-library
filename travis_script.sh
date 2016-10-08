#!/bin/sh
set -e
case "${TRAVIS_OS_NAME}" in
  linux)
	pwd && ls -l && rm -r debug && ls -l && mkdir -p debug && cd debug && cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" && make && make test
	cd ..
	pwd && mkdir -p release && cd release && cmake .. -G "Unix Makefiles" && make && make test
    ;;
  osx)
	./cmake_osx.sh osxbuild && cd ../osxbuild && xcodebuild -target ALL_BUILD
	# xcodebuild -target RUN_TESTS
    ;;
esac
