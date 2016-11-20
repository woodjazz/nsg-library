::-------------------------------------------------------------------------------
::This file is part of nsg-library.
::http://github.com/woodjazz/nsg-library
::
::Copyright (c) 2014-2016 Néstor Silveira Gorski
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

if "%ANDROID_NDK%" == "" (
	echo "Environment variable ANDROID_NDK shall be set."
	@exit /b 1
)

if "%1" == "" (
	echo "Enter build target directory. (directory will be created on the parent directory of the current one"
	@exit /b 1
)

cd ..
cmake -E make_directory %1
cd %1
@echo "*** CONFIGURING PROJECTS ***"
::cmake %SOURCE_FOLDER% -G "Unix Makefiles" -DBUILD_PROJECT="all" -DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-clang3.4" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows/bin/make.exe" -DCMAKE_TOOLCHAIN_FILE="%SOURCE_FOLDER%/CMake/Toolchains/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=%CD%
cmake %SOURCE_FOLDER% -G "Unix Makefiles" -DBUILD_PROJECT="all" -DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-clang3.4" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows/bin/make.exe" -DCMAKE_TOOLCHAIN_FILE="%SOURCE_FOLDER%/CMake/Toolchains/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=%CD%

::@echo "*** ANALYZING %2 ***"
:: FOR ANALYZING ALWAYS CONFIGURE IN DEBUG MODE
::set PATH=%PATH%;%ANDROID_NDK%/prebuilt/common/scan-build
::scan-build --use-analyzer %ANDROID_NDK%/toolchains/llvm-3.4/prebuilt/windows-x86_64/bin %ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe %2

@echo "*** BUILDING %2 ***"
%ANDROID_NDK%/prebuilt/windows/bin/make.exe %2

@echo "*** CLEARING LOGCAT ****"
cmd /C %ANDROID_SDK%/platform-tools/adb logcat -c

::@echo "*** STARTING test ***"
::cmd /C %ANDROID_SDK%/platform-tools/adb shell am start -a android.intent.action.MAIN -n com.nsg.sample000/android.app.sample000

@echo "*** FILTERING LOGCAT FOR nsg-library ***"
cmd /C %ANDROID_SDK%/platform-tools/adb logcat nsg-library AndroidRuntime *:S




