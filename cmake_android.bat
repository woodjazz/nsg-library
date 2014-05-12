rem @echo off

set SOURCE_FOLDER=%CD%

pushd %SOURCE_FOLDER%

if "%ANDROID_NDK%" == "" (
	echo "Environment variable ANDROID_NDK shall be set."
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

@echo "*** CONFIGURING PROJECTS ***"
rem cmake %SOURCE_FOLDER% -G "Unix Makefiles" -DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-clang3.4" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows/bin/make.exe" -DCMAKE_TOOLCHAIN_FILE="%SOURCE_FOLDER%/CMake/Toolchains/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=%CD%
cmake %SOURCE_FOLDER% -G "Unix Makefiles" -DANDROID_TOOLCHAIN_NAME="arm-linux-androideabi-clang3.4" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows/bin/make.exe" -DCMAKE_TOOLCHAIN_FILE="%SOURCE_FOLDER%/CMake/Toolchains/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=%CD%

@echo "*** MAKING ***"
%ANDROID_NDK%/prebuilt/windows/bin/make.exe 

rem popd

@echo "*** CLEARING LOGCAT ****"
cmd /C %ANDROID_SDK%/platform-tools/adb logcat -c

rem @echo "*** STARTING test ***"
rem cmd /C %ANDROID_SDK%/platform-tools/adb shell am start -a android.intent.action.MAIN -n com.nsg.sample000/android.app.sample000

@echo "*** FILTERING LOGCAT FOR nsg-library ***"
cmd /C %ANDROID_SDK%/platform-tools/adb logcat nsg-library *:S




