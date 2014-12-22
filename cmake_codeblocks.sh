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

cd $( dirname $0 ) # Ensure we are in project root directory
set -e
SOURCE_FOLDER="$PWD"

if [ ! -n "$1" ]; then
	echo "Enter build target directory. (directory will be created on the parent directory of the current one"
	exit 0
fi

cd ..
cmake -E make_directory $1
cd $1

cmake $SOURCE_FOLDER -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug"
#cmake $SOURCE_FOLDER -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE="Release"
