include( CMakeForceCompiler )

set(NACL                       ON )

set(PLATFORM_EMBEDDED          ON )
set(PLATFORM_NAME              "PNaCl" )
set(PLATFORM_TRIPLET           "pnacl" )
set(PLATFORM_PREFIX            "$ENV{NACL_SDK_ROOT}/toolchain/win_pnacl" )
set(PLATFORM_EXE_SUFFIX        ".pexe" )

set(CMAKE_SYSTEM_PROCESSOR     		"LLVM-IR" CACHE STRING "Target processor." )
set(CMAKE_AR                   		"${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ar" CACHE STRING "")
set(CMAKE_RANLIB               		"${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ranlib" CACHE STRING "")
set(CMAKE_C_COMPILER           		"${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-clang" )
set(CMAKE_CXX_COMPILER         		"${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-clang++" )
set(CMAKE_C_FLAGS              		"-DNACL -I$ENV{NACL_SDK_ROOT}/include -I$ENV{NACL_SDK_ROOT}/include/pnacl" CACHE STRING "" )
set(CMAKE_CXX_FLAGS            		"-DNACL -std=gnu++11 -stdlib=libc++ -I$ENV{NACL_SDK_ROOT}/include -I$ENV{NACL_SDK_ROOT}/include/pnacl" CACHE STRING "" )
set(CMAKE_EXE_LINKER_FLAGS_DEBUG 	"-L$ENV{NACL_SDK_ROOT}/lib/pnacl/Debug -lppapi -lppapi_cpp -lppapi_gles2 -lpthread -lnacl_io")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE 	"-L$ENV{NACL_SDK_ROOT}/lib/pnacl/Release -lppapi -lppapi_cpp -lppapi_gles2 -lpthread -lnacl_io")
# cmake_force_c_compiler(         ${CMAKE_C_COMPILER} Clang )
# cmake_force_cxx_compiler(       ${CMAKE_CXX_COMPILER} Clang )

# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY )

macro(pnacl_finalise _target )
  add_custom_command( TARGET ${_target}
    POST_BUILD COMMAND "${PLATFORM_PREFIX}/bin64/${PLATFORM_TRIPLET}-finalize"
                       "$<TARGET_FILE:${_target}>" )
endmacro()

include_directories( SYSTEM $ENV{NACL_SDK_ROOT}/include )
link_directories( $ENV{NACL_SDK_ROOT}/lib/pnacl/Release )


#cmake -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM="%NACL_SDK_ROOT%/tools/make.exe" -DCMAKE_TOOLCHAIN_FILE="../nsg-library/PNaCl.Windows.cmake" ../nsg-library