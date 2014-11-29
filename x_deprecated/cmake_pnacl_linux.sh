#!/bin/bash
#-------------------------------------------------------------------------------
#This file is part of nsg-library.
#http://nsg-library.googlecode.com/
#
#Copyright (c) 2014-2015 Néstor Silveira Gorski
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

set -e
export SOURCE_FOLDER="$PWD"

if [ ! -n "$NACL_SDK_ROOT" ]; then
    echo "Environment variable NACL_SDK_ROOT shall be set."
    exit 0
fi

if [ ! -n "$1" ]; then
	echo "Enter build target directory. (directory will be created on the parent directory of the current one"
	exit 0
fi

cd ..
cmake -E make_directory $1
cd $1

cmake $SOURCE_FOLDER -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="$SOURCE_FOLDER/cmake/toolchains/PNaCl.Linux.cmake"
make $2

@echo "Starting HTTP Server"
python $NACL_SDK_ROOT/tools/httpd.py --no_dir_check -C $PWD/tests/generic_test
rem python %NACL_SDK_ROOT%/tools/httpd.py --no_dir_check -C $PWD/samples/sample000
