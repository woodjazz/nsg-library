@echo off

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

if not exist %1 (
	mkdir %1
)

cd %1

cmake %SOURCE_FOLDER% -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%/prebuilt/windows/bin/make.exe" -DCMAKE_TOOLCHAIN_FILE="%SOURCE_FOLDER%/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=android-19 -DLIBRARY_OUTPUT_PATH_ROOT=./Test/AndroidHost
%ANDROID_NDK%/prebuilt/windows/bin/make.exe 
cd Test/AndroidHost
%ANT_HOME%/bin/ant debug
%ANDROID_SDK%/platform-tools/adb -d install -r bin/test-debug.apk
%ANDROID_SDK%/platform-tools/adb logcat nsg-library *:S
rem to debug:
rem %ANDROID_SDK%/platform-tools/adb shell
rem am start -a android.intent.action.MAIN -n com.nsg.test/android.app.NativeActivity


