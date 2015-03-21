#!/bin/sh
set -e
<<<<<<< HEAD
case "${TRAVIS_OS_NAME}" in
  linux)
  	./cmake_linux.sh linuxbuild && cd ../linuxbuild && make && make test
    ;;
  osx)
	./cmake_osx.sh osxbuild && cd ../osxbuild && xcodebuild -target ALL_BUILD && xcodebuild -target RUN_TESTS
    ;;
esac
=======
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
cd tests
for testdir in *; do
    if [ -d $testdir ]; then
        echo && echo RUNNING $testdir && echo && cd $testdir && ./$testdir && cd ..
    fi
done
>>>>>>> origin/master
