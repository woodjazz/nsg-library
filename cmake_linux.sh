#!/bin/bash
#-------------------------------------------------------------------------------
#This file is part of nsg-library.
#http://github.com/woodjazz/nsg-library
#
#Copyright (c) 2014-2017 Néstor Silveira Gorski
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

PrintUsage()
{
cat <<EOM

        Usage: $0 [OPTION]

            --target    <build directory/(build_)>
            --type      <debug/(release)>
            --platform  <emscripten/android/ios/osx/yocto/(linux)>

EOM
}

TARGET_DIR_="$PWD/build_"
TYPE_=release
OPTS_=
GENERATOR_="Unix Makefiles"

SetupEmscripten()
{
    if [ ! -n "$HOME_EMSCRIPTEN" ]; then
        echo "Environment variable HOME_EMSCRIPTEN shall be set."
        exit 0
    fi
    cd $HOME_EMSCRIPTEN
    source $HOME_EMSCRIPTEN/emsdk_env.sh
    #$HOME_EMSCRIPTEN/emsdk activate 
    cd $SOURCE_FOLDER
    OPTS_=-DCMAKE_TOOLCHAIN_FILE="$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake"
}

SetupAndroid()
{
    if [ ! -n "$ANDROID_NDK" ]; then
        echo "Environment variable ANDROID_NDK shall be set."
        exit 0
    fi
	
    OPTS_=-DANDROID_STL=gnustl_shared \
        -DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/cmake/toolchains/android.toolchain.cmake" \
        -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 \
        -DANDROID_NATIVE_API_LEVEL=android-19 \
        -DLIBRARY_OUTPUT_PATH_ROOT=$PWD
        
}

SetupIOS()
{
    GENERATOR_="Xcode"
    OPTS_="-DIOS=ON"
}

SetupOSX()
{
    GENERATOR_="Xcode"
}

SetupYocto()
{
    if [ -z "$SDKTARGETSYSROOT" ]; then
        echo "Remember to source the yocto SDK environment before calling this script"
        PrintUsage
        exit 1
    fi

    OPTS_=-DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/cmake/toolchains/yocto.toolchain.cmake" \
        #-DOE_QMAKE_PATH_EXTERNAL_HOST_BINS="$OE_QMAKE_PATH_HOST_BINS" 

}

SetupPlatform()
{
    case "$1" in
        "emscripten")
            SetupEmscripten 
            ;;
        "android")
            SetupAndroid 
            ;;
        "ios")
            SetupIOS
            ;;
        "osx")
            SetupOSX
            ;;
        "yocto")
            SetupYocto
            ;;
        "linux")
            ;;
        *)
            PrintUsage
            exit 0
            ;;
    esac 
}

GetParameters()
{
    while test -n "$1"; do
      case "$1" in
        "--target")
            shift
            TARGET_DIR_=$1
            ;;
        "--type")
            shift
            TYPE_=$1
            ;;
        "--platform")
            shift
            SetupPlatform $1
            ;;
        *)
            PrintUsage
            exit 0
            ;;
      esac
      shift
    done
}

GetParameters "$@"

echo "*****************************"
echo "Building $TYPE_"
echo "dir=$TARGET_DIR_"
echo "*****************************"

cmake -E remove_directory $TARGET_DIR_
cmake -E make_directory $TARGET_DIR_

cd $TARGET_DIR_

#In order for find_package to be successful, Qt 5 must be found below the 
#CMAKE_PREFIX_PATH, or the Qt5<Module>_DIR must be set in the CMake cache 
#to the location of the Qt5WidgetsConfig.cmake file. The easiest way to use 
#CMake is to set the CMAKE_PREFIX_PATH environment variable to the install 
#prefix of Qt 5.
#(see http://doc.qt.io/qt-5/cmake-manual.html)
cmake -G "$GENERATOR_" -DCMAKE_BUILD_TYPE="$TYPE_" ${OPTS_} $SOURCE_FOLDER 

