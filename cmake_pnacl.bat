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

if "%NACL_SDK_ROOT%" == "" (
	echo "Environment variable NACL_SDK_ROOT shall be set."
	@exit /b 1
)

if "%1" == "" (
	echo "Enter build target directory. (directory will be created on the parent directory of the current one"
	@exit /b 1
)

cd ..
cmake -E make_directory %1
cd %1

cmake %SOURCE_FOLDER% -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM="%NACL_SDK_ROOT%/tools/make.exe" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_TOOLCHAIN_FILE="%SOURCE_FOLDER%/cmake/toolchains/PNaCl.Windows.cmake"
::cmake %SOURCE_FOLDER% -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM="%NACL_SDK_ROOT%/tools/make.exe" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_TOOLCHAIN_FILE="%SOURCE_FOLDER%/cmake/toolchains/PNaCl.Windows.cmake"
%NACL_SDK_ROOT%/tools/make.exe %2

@echo "Starting HTTP Server"
::python %NACL_SDK_ROOT%/tools/httpd.py --no_dir_check -C %CD%/tests/generic_test
::python %NACL_SDK_ROOT%/tools/httpd.py --no_dir_check -C %CD%/samples/sample000
::python %NACL_SDK_ROOT%/tools/httpd.py --no_dir_check -C %CD%/samples/filters
::python %NACL_SDK_ROOT%/tools/httpd.py --no_dir_check -C %CD%/samples/imgui
::python %NACL_SDK_ROOT%/tools/httpd.py --no_dir_check -C %CD%/samples/earth
::python %NACL_SDK_ROOT%/tools/httpd.py --no_dir_check -C %CD%/samples/batches
