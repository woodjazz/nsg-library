#!/bin/bash
set -e
export SOURCE_FOLDER="$PWD"

if [ ! -n "$1" ]; then
	echo "Enter build target directory. (directory will be created on the parent directory of the current one"
	exit 0
fi

cd ..

if [ ! -d "$1" ]; then
	mkdir $1
fi

cd $1

cmake $SOURCE_FOLDER -G "Xcode" -DIOS=ON
