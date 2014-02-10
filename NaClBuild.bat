@echo off

set SOURCE_FOLDER=%CD%

if %NACL_SDK_ROOT% == "" (
	echo "Environment variable NACL_SDK_ROOT shall be set."
	@exit /b 1
)

if "%1" == "" (
	echo "Enter build target directory. (directory will be created on the parent directory of the current one"
	@exit /b 1
)

cd ..

if not exist %1 (
	mkdir %1
)

cd %1

cmake %SOURCE_FOLDER% -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM="%NACL_SDK_ROOT%/tools/make.exe" -DCMAKE_TOOLCHAIN_FILE="%SOUCE_FOLDER%/PNaCl.Windows.cmake"
%NACL_SDK_ROOT%/tools/make.exe 
