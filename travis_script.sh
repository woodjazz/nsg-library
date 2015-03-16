#!/bin/sh
set -e
  # EMSCRIPTEN BUILD AND TEST
  # - ./cmake_ems.sh emsbuild && cd ../emsbuild && make && make test
case "${TRAVIS_OS_NAME}" in
  linux)
  	./cmake_linux.sh linuxbuild && cd ../linuxbuild && make
  	#make test
    ;;
  osx)
	./cmake_osx.sh osxbuild && cd ../osxbuild && xcodebuild -target ALL_BUILD
	#xcodebuild -target RUN_TESTS
    ;;
esac
# Run tests
pushd .
cd tests
for fullfile in *; do
    if [ -d $fullfile ]; then
        filename="${fullfile##*/}"
        echo && echo RUNNING ./$fullfile/$filename
        cd $fullfile
        ./$filename
        cd ..
    fi
done
popd
