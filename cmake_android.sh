#!/bin/bash
set -e

SOURCE_FOLDER="$PWD"

pushd ${SOURCE_FOLDER}

if [ ! -n "$ANDROID_NDK" ]; then
    echo "Environment variable ANDROID_NDK shall be set."
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

echo "*** CONFIGURING PROJECTS ***"
#cmake $SOURCE_FOLDER -G "Unix Makefiles" -DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-clang3.4" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/CMake/Toolchains/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=./Test/AndroidHost
cmake $SOURCE_FOLDER -G "Unix Makefiles"  -DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-clang3.4" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/CMake/Toolchains/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=./Test/AndroidHost

echo "*** MAKING ***"
make

cd Test/AndroidHost

echo "*** CREATING APK ***"
$ANT_HOME/bin/ant debug

echo "*** INSTALLING APK ON DEVICE ***"
$ANDROID_SDK/platform-tools/adb -d install -r bin/test-debug.apk

popd

echo "*** CLEARING LOGCAT ****"
$ANDROID_SDK/platform-tools/adb logcat -c

echo "*** STARTING test ***"
$ANDROID_SDK/platform-tools/adb shell am start -a android.intent.action.MAIN -n com.nsg.test/android.app.NativeActivity

echo "*** FILTERING LOGCAT FOR nsg-library ***"
$ANDROID_SDK/platform-tools/adb logcat nsg-library *:S
