# /*
# -------------------------------------------------------------------------------
# This file is part of nsg-library.
# http://nsg-library.googlecode.com/

# Copyright (c) 2014-2015 Néstor Silveira Gorski

# -------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.

# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:

# 1. The origin of this software must not be misrepresented; you must not
# claim that you wrote the original software. If you use this software
# in a product, an acknowledgment in the product documentation would be
# appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
# misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
# -------------------------------------------------------------------------------
# */
cmake_minimum_required(VERSION 2.8.4)

PROJECT(nsg-library)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Include necessary submodules
set(CMAKE_MODULE_PATH 
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake"
)

if(WIN32)

	if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
		add_definitions("/Zi /wd4519")
		add_definitions( "/MP" )
	endif()

	if (MSVC)
		add_definitions( -D_CRT_SECURE_NO_WARNINGS )
		add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
		add_definitions( -DNOMINMAX)

		#We statically link to reduce dependancies
		FOREACH(flag_var    CMAKE_CXX_FLAGS 
							CMAKE_CXX_FLAGS_DEBUG 
							CMAKE_CXX_FLAGS_RELEASE 
							CMAKE_CXX_FLAGS_MINSIZEREL 
							CMAKE_CXX_FLAGS_RELWITHDEBINFO
							CMAKE_C_FLAGS_DEBUG
							CMAKE_C_FLAGS_RELEASE
							CMAKE_C_FLAGS_MINSIZEREL 
							CMAKE_C_FLAGS_RELWITHDEBINFO
							)
			IF(${flag_var} MATCHES "/MD")
				STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
			ENDIF(${flag_var} MATCHES "/MD")
			IF(${flag_var} MATCHES "/MDd")
				STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
			ENDIF(${flag_var} MATCHES "/MDd")
		ENDFOREACH(flag_var)

	ENDIF(MSVC)

endif(WIN32)

if (CMAKE_CXX_COMPILER MATCHES ".*clang[++]")
	set(CMAKE_COMPILER_IS_CLANGXX 1)
endif ()

if(CMAKE_COMPILER_IS_CLANGXX)
	message("detected clang compiler")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
elseif(CMAKE_COMPILER_IS_GNUCXX)
	message("detected GNU compiler")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
	SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()

macro ( mark_as_disable_and_internal _var )
	set ( ${_var} OFF CACHE INTERNAL "disable and hide this!" FORCE )
endmacro( mark_as_disable_and_internal _var )

macro ( mark_as_enable _var )
	set ( ${_var} ON CACHE BOOL "enable this!" FORCE )
endmacro( mark_as_enable _var )


include_directories(${CMAKE_CURRENT_SOURCE_DIR})


set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if(NOT NACL AND NOT ANDROID)

	find_package(OpenGL REQUIRED)
	###########################
	# glew
	###########################

	add_definitions(-DGLEW_STATIC)
	set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
	include_directories(${GLEW_INCLUDE_DIR})
	add_subdirectory(glew)
	list(APPEND LIBRARIES_2_LINK glew) 

	###########################
	# glfw
	###########################
	set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
	mark_as_disable_and_internal(USE_MSVC_RUNTIME_LIBRARY_DLL)
	mark_as_disable_and_internal(GLFW_BUILD_EXAMPLES)
	mark_as_disable_and_internal(GLFW_BUILD_TESTS)
	mark_as_disable_and_internal(GLFW_INSTALL)
	add_subdirectory(glfw)
	include_directories(${GLFW_INCLUDE_DIR})
	list(APPEND LIBRARIES_2_LINK glfw) 

else()

	if(WIN32)
		set(CMAKE_LINK_LIBRARY_SUFFIX "")
	endif()	

	if(NACL)
		set(CMAKE_EXECUTABLE_SUFFIX ".pexe")
	endif()

endif()

###########################
# soil
###########################
set(SOIL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/soil)
add_subdirectory(soil)
list(APPEND LIBRARIES_2_LINK soil) 

###########################
# freetype2
###########################
set(FREETYPE2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/freetype2/include)
include_directories(${FREETYPE2_INCLUDE_DIR})
add_subdirectory(freetype2)
list(APPEND LIBRARIES_2_LINK freetype)
add_definitions(-DFREETYPE)

###########################
# codecs
###########################
#add_subdirectory(codecs)
#list(APPEND LIBRARIES_2_LINK codecs) 

###########################
# stb
###########################
# add_subdirectory(stb)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stb)
# list(APPEND LIBRARIES_2_LINK stb) 

###########################
# assimp
###########################
macro ( generate_memory_h)
	if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/assimp/memory.h")
		set (LOCAL_PROPERTIES_FILE ${CMAKE_CURRENT_BINARY_DIR}/assimp/memory.h)
		file(WRITE ${LOCAL_PROPERTIES_FILE} "//Autogenerated by cmake\n")
		file(APPEND ${LOCAL_PROPERTIES_FILE} "#pragma once\n")
		file(APPEND ${LOCAL_PROPERTIES_FILE} "#include <memory>\n")
#		file(APPEND ${LOCAL_PROPERTIES_FILE} "#include <algorithm>\n")
#		file(APPEND ${LOCAL_PROPERTIES_FILE} "#include <functional>\n")
		file(APPEND ${LOCAL_PROPERTIES_FILE} "extern char* realpath(const char* in, char* out);\n")
	endif()
endmacro( generate_memory_h)

if(NACL)
	#include <memory.h> is not found on NACL
	generate_memory_h() # this solves the issue
	include_directories(${CMAKE_CURRENT_BINARY_DIR}/assimp)
endif(NACL)

#add_definitions( -DASSIMP_BUILD_BOOST_WORKAROUND )
mark_as_disable_and_internal(BUILD_SHARED_LIB)
mark_as_enable(ASSIMP_ENABLE_BOOST_WORKAROUND)
mark_as_disable_and_internal(BUILD_SHARED_LIBS)
mark_as_disable_and_internal(ASSIMP_BUILD_ASSIMP_TOOLS)
set(ASSIMP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include)
include_directories(${ASSIMP_INCLUDE_DIR})
add_subdirectory(assimp)
list(APPEND LIBRARIES_2_LINK assimp) 

###########################
# rtaudio
###########################
# if(WIN32)
# 	find_package(DirectSound REQUIRED)
# 	include_directories(${DIRECTSOUND_INCLUDE_DIRS})
# 	add_definitions(-D__WINDOWS_DS__)
# endif()

# include_directories(codecs/vorbis/include)
# include_directories(codecs/ogg/include)
# add_subdirectory(rtaudio)

###########################
# alure
###########################
# add_definitions(-DAL_LIBTYPE_STATIC)
# set(ALURE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/alure/include ${CMAKE_CURRENT_SOURCE_DIR}/alure/include/AL)
# mark_as_enable(BUILD_STATIC)
# mark_as_disable_and_internal(BUILD_SHARED)
# mark_as_disable_and_internal(BUILD_EXAMPLES)
# mark_as_disable_and_internal(DYNLOAD)
# set(OPENAL_FOUND TRUE FORCE)
# #include_directories(${OPENAL_SOFT_INCLUDE_DIRS})

# IF(WIN32)
#     SET(ALURE_LIBRARIES ALURE32-static)
# ELSE(WIN32)
#     SET(ALURE_LIBRARIES alure-static)
# ENDIF(WIN32)

# add_definitions(-DALURE_STATIC_LIBRARY)
# add_subdirectory(alure)


###########################
# nsg
###########################
set(NSG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nsg)
include_directories(${NSG_INCLUDE_DIR})
add_subdirectory(NSG)

###########################
# test
###########################
add_subdirectory(Test)




