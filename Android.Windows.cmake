set(ANDROID                    ON )
set(PLATFORM_EMBEDDED          ON )
set(PLATFORM_NAME              "Android" )
set(PLATFORM_PREFIX            "$ENV{NDK}/../android-toolchain" )
#set(PLATFORM_FLAGS             "--sysroot=${PLATFORM_PREFIX}/sysroot" )
set(PLATFORM_TRIPLET			"i686-linux-android")

#set(CMAKE_SYSTEM_NAME          "Linux" CACHE STRING "Target system." )
#set(CMAKE_SYSTEM_PROCESSOR     "i686" CACHE STRING "Target processor." )
#set(CMAKE_FIND_ROOT_PATH       ${PLATFORM_PORTS_PREFIX};${PLATFORM_PREFIX} )
set(CMAKE_AR                   	${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ar.bat CACHE STRING "")
set(CMAKE_RANLIB               	${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ranlib.bat CACHE STRING "")
set(CMAKE_C_COMPILER           	${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-gcc)
set(CMAKE_CXX_COMPILER         	${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-g++)
set(CMAKE_C_FLAGS              	"-DANDROID ${PLATFORM_FLAGS} -I${PLATFORM_PREFIX}/include" CACHE STRING "" )
set(CMAKE_CXX_FLAGS            	"-DANDROID ${PLATFORM_FLAGS} -I${PLATFORM_PREFIX}/include" CACHE STRING "" )

#set( CMAKE_C_FLAGS              "${PLATFORM_FLAGS} -msse3 -mfpmath=sse" CACHE STRING "" )
#set( CMAKE_CXX_FLAGS            "${PLATFORM_FLAGS} -msse3 -mfpmath=sse" CACHE STRING "" )
#set( CMAKE_C_FLAGS_RELEASE      "-Ofast" CACHE STRING "" )
#set( CMAKE_CXX_FLAGS_RELEASE    "-Ofast" CACHE STRING "" )

# set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
# set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
# set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
# set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY )

# include_directories( SYSTEM ${PLATFORM_STL_PREFIX}/include )
# include_directories( SYSTEM ${PLATFORM_STL_PREFIX}/libs/x86/include )

#$NDK/build/tools/make-standalone-toolchain.sh --platform=android-9 --install-dir=$HOME/dev/android-toolchain --toolchain=x86-4.8 --system=windows

#cmake.exe -G "Unix Makefiles" _DANDROID_NDK=%NDK% -DANDROID_STANDALONE_TOOLCHAIN=%NDK%/../android-toolchain -DCMAKE_MAKE_PROGRAM="%NDK%/prebuild/windows/bin/make.exe" -DCMAKE_TOOLCHAIN_FILE="../nsg-library/android.toolchain.cmake" -DANDROID_NATIVE_API_LEVEL=9 ../nsg-library

#cmake.exe -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM="%NDK%/prebuild/windows/bin/make.exe" -DCMAKE_TOOLCHAIN_FILE="../nsg-library/Android.Windows.cmake" -DANDROID_NATIVE_API_LEVEL=9 ../nsg-library
#cmake.exe -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE="../nsg-library/Android.Windows.cmake" -DANDROID_NATIVE_API_LEVEL=9 ../nsg-library
#make
