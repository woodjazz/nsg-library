-------------------------
NACL
-------------------------
1- Install SDK
	https://developers.google.com/native-client/dev/sdk/download
2- Set the environment variable NACL_SDK_ROOT
	Windows: set NACL_SDK_ROOT=e:/nacl_sdk/pepper_canary/pepper_34 (better set in systsem properties)
	Linux: export NACL_SDK_ROOT=$HOME/dev/nacl_sdk/pepper_canary (better edit .profile)
3- Build
	Linux: 
		NaClBuild.sh [output-dir]
	Windows:
		NaClBuild.bat [output-dir]

---------------------------
ANDROID
---------------------------
1- Install SDK and NDK
2- Set the environment variable NDK 	
3- Make the toolchain:
	Linux:
		$NDK/build/tools/make-standalone-toolchain.sh --toolchain=arm-linux-androideabi-4.8 
		--platform=android-9 --install-dir=$HOME/dev/android-toolchain --llvm-version=3.3	