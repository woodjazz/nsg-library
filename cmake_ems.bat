::-------------------------------------------------------------------------------
::This file is part of nsg-library.
::http://nsg-library.googlecode.com/
::
::Copyright (c) 2014-2015 Néstor Silveira Gorski
::
::-------------------------------------------------------------------------------
::This software is provided 'as-is', without any express or implied
::warranty. In no event will the authors be held liable for any damages
::arising from the use of this software.
::
::Permission is granted to anyone to use this software for any purpose,
::including commercial applications, and to alter it and redistribute it
::freely, subject to the following restrictions:
::
::1. The origin of this software must not be misrepresented; you must not
::claim that you wrote the original software. If you use this software
::in a product, an acknowledgment in the product documentation would be
::appreciated but is not required.
::2. Altered source versions must be plainly marked as such, and must not be
::misrepresented as being the original software.
::3. This notice may not be removed or altered from any source distribution.
::-------------------------------------------------------------------------------

@echo off
pushd %~dp0
set SOURCE_FOLDER=%CD%

if "%EMSCRIPTEN%" == "" (
	echo "Environment variable EMSCRIPTEN shall be set."
	@exit /b 1
)

cmd /C emsdk_env

if "%1" == "" (
	echo "Enter build target directory. (directory will be created on the parent directory of the current one"
	@exit /b 1
)

cd ..
cmake -E make_directory %1
cd %1

@echo "*** CONFIGURING PROJECTS ***"
::cmake %SOURCE_FOLDER% -G "MinGW Makefiles" -DBUILD_PROJECT="all" -DEMS_DEBUG_LEVEL=4 -DALLOW_MEMORY_GROWTH=1 -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_TOOLCHAIN_FILE="%EMSCRIPTEN%/cmake/Platform/Emscripten.cmake"
cmake %SOURCE_FOLDER% -G "MinGW Makefiles" -DBUILD_PROJECT="all" -DALLOW_MEMORY_GROWTH=1 -DCMAKE_BUILD_TYPE="Release" -DCMAKE_TOOLCHAIN_FILE="%EMSCRIPTEN%/cmake/Platform/Emscripten.cmake"

@echo "*** BUILDING %2 ***"
::%MINGW_ROOT%/bin/mingw32-make
%NACL_SDK_ROOT%/tools/make.exe %2
