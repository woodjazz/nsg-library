#!/bin/bash
set -e
export SOURCE_FOLDER="$PWD"

if [ ! -n "$ANDROID_TOOLCHAIN_ROOT" ]; then
    echo "Environment variable ANDROID_TOOLCHAIN_ROOT shall be set."
    exit 0
fi

if [ ! -n "$1" ]; then
	echo "Enter build target directory. (directory will be created on the parent directory of the current one"
	exit 0
fi

cd ..

if [ ! -d "$1" ]; then
	mkdir $1
fi

cd $1

#-DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-clang3.4"
#cmake $SOURCE_FOLDER -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=./Test/AndroidHost
cmake $SOURCE_FOLDER -G "Unix Makefiles"  -DCMAKE_BUILD_TYPE="Release" -DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=./Test/AndroidHost
make
cd Test/AndroidHost
$ANT_HOME/bin/ant debug
$ANDROID_SDK/platform-tools/adb -d install -r bin/test-debug.apk
$ANDROID_SDK/platform-tools/adb logcat -c
$ANDROID_SDK/platform-tools/adb shell am start -a android.intent.action.MAIN -n com.nsg.test/android.app.NativeActivity
$ANDROID_SDK/platform-tools/adb logcat nsg-library *:S
