include( CMakeForceCompiler )

set(ANDROID                     	ON )
set(PLATFORM_EMBEDDED          		ON )
set(PLATFORM_NAME              		"Android" )
set(PLATFORM_TRIPLET           		"arm-linux-androideabi" )
set(PLATFORM_PREFIX            		"$ENV{ANDROID_TOOLCHAIN_ROOT}" )
set(CMAKE_AR                   		${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ar CACHE STRING "")
set(CMAKE_RANLIB               		${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ranlib CACHE STRING "")
set(CMAKE_C_COMPILER           		${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-clang CACHE STRING "")
set(CMAKE_CXX_COMPILER         		${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-clang++ CACHE STRING "")
set(CMAKE_C_FLAGS              		"-DANDROID" CACHE STRING "")
set(CMAKE_CXX_FLAGS            		"-DANDROID -std=c++11" CACHE STRING "")
#set(CMAKE_EXE_LINKER_FLAGS_DEBUG 	"-L$ENV{NACL_SDK_ROOT}/lib/pnacl/Debug")
#set(CMAKE_EXE_LINKER_FLAGS_RELEASE 	"-L$ENV{NACL_SDK_ROOT}/lib/pnacl/Release")

include_directories( SYSTEM $ENV{ANDROID_TOOLCHAIN_SDK}/sysroot/usr/include)
include_directories( SYSTEM $ENV{NDK}/platforms/android9/arch-arm/usr/include)
link_directories($ENV{NDK}/platforms/android9/arch-arm/usr/lib)

#$NDK/build/tools/make-standalone-toolchain.sh --toolchain=arm-linux-androideabi-clang3.3 --platform=android-9 --install-dir=$HOME/dev/android-toolchain --llvm-version=3.3