#!/bin/bash
#-------------------------------------------------------------------------------
#This file is part of nsg-library.
#http://github.com/woodjazz/nsg-library
#
#Copyright (c) 2014-2016 Néstor Silveira Gorski
#
#-------------------------------------------------------------------------------
#This software is provided 'as-is', without any express or implied
#warranty. In no event will the authors be held liable for any damages
#arising from the use of this software.
#
#Permission is granted to anyone to use this software for any purpose,
#including commercial applications, and to alter it and redistribute it
#freely, subject to the following restrictions:
#
#1. The origin of this software must not be misrepresented; you must not
#claim that you wrote the original software. If you use this software
#in a product, an acknowledgment in the product documentation would be
#appreciated but is not required.
#2. Altered source versions must be plainly marked as such, and must not be
#misrepresented as being the original software.
#3. This notice may not be removed or altered from any source distribution.
#-------------------------------------------------------------------------------

cd $( dirname $0 ) # Ensure we are in project root directory
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
cmake -E make_directory $1
cd $1

echo "*** CONFIGURING PROJECTS ***"
if [ $2 = "debug" ]; then
	echo "Building DEBUG version..."
	cmake $SOURCE_FOLDER -G "Unix Makefiles" -DANDROID_STL=gnustl_shared -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/cmake/toolchains/android.toolchain.cmake" -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=$PWD 
else	
	echo "Building RELEASE version..."
	cmake $SOURCE_FOLDER -G "Unix Makefiles" -DANDROID_STL=gnustl_shared -DCMAKE_BUILD_TYPE="Release" -DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/cmake/toolchains/android.toolchain.cmake" -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=$PWD
fi

echo "*** BUILDING $2 ***"
make $3

#$ANDROID_NDK/ndk-gdb

echo "*** CLEARING LOGCAT ****"
$ANDROID_SDK/platform-tools/adb logcat -c

#echo "*** STARTING test ***"
#$ANDROID_SDK/platform-tools/adb shell am start -a android.intent.action.MAIN -n com.nsg.test/android.app.NativeActivity

echo "*** FILTERING LOGCAT FOR nsg-library ***"
$ANDROID_SDK/platform-tools/adb logcat nsg-library *:S
#$ANDROID_SDK/platform-tools/adb logcat
