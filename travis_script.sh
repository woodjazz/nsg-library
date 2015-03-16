#!/bin/sh
set -e
  # EMSCRIPTEN BUILD AND TEST
  # - ./cmake_ems.sh emsbuild && cd ../emsbuild && make && make test
case "${TRAVIS_OS_NAME}" in
  linux)
    ./cmake_linux.sh linuxbuild && cd ../linuxbuild && make && make test
    ;;
  osx)
	./cmake_osx.sh osxbuild && cd ../osxbuild && xcodebuild -target ALL_BUILD && xcodebuild -target RUN_TESTS
    ;;
esac

# Run tests
for fullfile in tests/*; do
    if [[ -d $fullfile ]]; then
        filename="${fullfile##*/}"
        ./$fullfile/$filename
    fi
done
