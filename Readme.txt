-------------------------
NACL
-------------------------
1- Install SDK
	https://developers.google.com/native-client/dev/sdk/download
2- Set the environment variable NACL_SDK_ROOT
3- Build
	Linux: 
		NaClBuild.sh [output-dir]
	Windows:
		NaClBuild.bat [output-dir]

---------------------------
ANDROID
---------------------------
(cmake toolchain from: https://github.com/taka-no-me/android-cmake   [replace 4.6 for 4.8])
1- Install SDK,NDK,Java
2- Install ant (http://ant.apache.org/bindownload.cgi)
	Linux also needed: sudo apt-get install openjdk-7-jdk
3- Set the environment variables ANDROID_SDK, ANDROID_NDK, ANT_HOME and JAVA_HOME
4- Make the toolchain (linux only):
	$NDK/build/tools/make-standalone-toolchain.sh --toolchain=arm-linux-androideabi-4.8 --platform=android-9 --install-dir=$HOME/dev/android-toolchain --llvm-version=3.3
5- Build
	Linux: 
		AndroidBuild.sh [output-dir]
	Windows
		AndroidBuild.bat [output-dir]
