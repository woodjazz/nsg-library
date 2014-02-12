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
1- Install SDK and NDK
2- Install ant (http://ant.apache.org/bindownload.cgi)
3- Set the environment variables ANDROID_SDK, ANDROID_NDK and ANT_HOME
4- Make the toolchain (linux only):
	$NDK/build/tools/make-standalone-toolchain.sh --toolchain=arm-linux-androideabi-4.8 --platform=android-9 --install-dir=$HOME/dev/android-toolchain --llvm-version=3.3
5- Build
	Linux: 
		AndroidBuild.sh [output-dir]
	Windows
		AndroidBuild.bat [output-dir]
