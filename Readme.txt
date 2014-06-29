-------------------------
NACL
-------------------------
1- Install SDK
	https://developers.google.com/native-client/dev/sdk/download
2- Set the environment variable NACL_SDK_ROOT
	2.1- Linux: edit ~/.bash_profile with EXPORT NACL_SDK_ROOT=~/nacl_sdk/pepper_xx
3- Build
	Linux: 
		cmake_nacl_linux.sh [output-dir]
	Windows:
		cmake_nacl_windows.bat [output-dir]

---------------------------
ANDROID
---------------------------
(cmake toolchain from: https://github.com/taka-no-me/android-cmake   [replace 4.6 for 4.8])
1- Install SDK,NDK,Java
2- Install ant (http://ant.apache.org/bindownload.cgi)
	Linux also needed: sudo apt-get install openjdk-7-jdk
3- Set the environment variables ANDROID_SDK, ANDROID_NDK, ANT_HOME and JAVA_HOME
4- Build
	Linux: 
		cmake_android.sh [output-dir]
	Windows
		cmake_android.bat [output-dir]

-------------------------
Linux (Debian/Ubuntu)
-------------------------
apt-get install xorg-dev
apt-get install libglu1-mesa-dev
