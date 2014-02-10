include( CMakeForceCompiler )

set(NACL                       		ON )
set(PLATFORM_EMBEDDED          		ON )
set(PLATFORM_NAME              		"PNaCl" )
set(PLATFORM_TRIPLET           		"pnacl" )
set(PLATFORM_PREFIX            		"$ENV{NACL_SDK_ROOT}/toolchain/linux_pnacl" )
set(CMAKE_AR                   		${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ar CACHE STRING "")
set(CMAKE_RANLIB               		${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ranlib CACHE STRING "")
set(CMAKE_C_COMPILER           		${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-clang  CACHE STRING "")
set(CMAKE_CXX_COMPILER         		${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-clang++  CACHE STRING "")
set(CMAKE_C_FLAGS              		"-DNACL -I$ENV{NACL_SDK_ROOT}/include -I$ENV{NACL_SDK_ROOT}/include/pnacl" CACHE STRING "" )
set(CMAKE_CXX_FLAGS            		"-DNACL -std=gnu++11 -stdlib=libc++ -I$ENV{NACL_SDK_ROOT}/include -I$ENV{NACL_SDK_ROOT}/include/pnacl" CACHE STRING "" )
set(CMAKE_EXE_LINKER_FLAGS_DEBUG 	"-L$ENV{NACL_SDK_ROOT}/lib/pnacl/Debug")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE 	"-L$ENV{NACL_SDK_ROOT}/lib/pnacl/Release")

macro(pnacl_finalise _target )
  add_custom_command( TARGET ${_target}
    POST_BUILD COMMAND "${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-finalize"
                       "$<TARGET_FILE:${_target}>" )
endmacro()

#include_directories( SYSTEM $ENV{NACL_SDK_ROOT}/include )
link_directories(debug $ENV{NACL_SDK_ROOT}/lib/pnacl/Debug)
link_directories(optimized $ENV{NACL_SDK_ROOT}/lib/pnacl/Release)
