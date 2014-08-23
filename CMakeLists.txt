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

if(NOT BUILD_PROJECT)
	set(BUILD_PROJECT "all")
endif()	

set(CMAKE_VERBOSE_MAKEFILE ON)

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

# Include NSG cmake common module
include (Common)
setup_common() #has to be called before anything

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(USE_SDL "Use SDL" ON)

#set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
set(DEPENDENCIES_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependencies)
set(EXTERNALS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externals)
set(EXTERNALS_BYNARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/externals)

###########################
# tinyxml2
###########################
set(TINYXML2_INCLUDE_DIR ${EXTERNALS_DIR}/tinyxml2)
include_directories(${TINYXML2_INCLUDE_DIR})
#add_subdirectory(${EXTERNALS_DIR}/tinyxml2)
#list(APPEND LIBRARIES_2_LINK tinyxml2)

###########################
# soil
###########################
add_subdirectory(${DEPENDENCIES_DIR}/soil)
include_directories(${DEPENDENCIES_DIR}/soil) 
list(APPEND LIBRARIES_2_LINK soil) 

if(NOT IS_TARGET_MOBILE AND NOT IS_TARGET_WEB)
	
	if(NOT USE_SDL)
		###########################
		# glfw
		###########################
		mark_as_disable_and_internal(USE_MSVC_RUNTIME_LIBRARY_DLL)
		mark_as_disable_and_internal(GLFW_BUILD_EXAMPLES)
		mark_as_disable_and_internal(GLFW_BUILD_TESTS)
		mark_as_disable_and_internal(GLFW_INSTALL)
		add_subdirectory(${EXTERNALS_DIR}/glfw)
		include_directories(${EXTERNALS_DIR}/glfw/include)
		list(APPEND LIBRARIES_2_LINK glfw)
	endif()

	##################################
	# glew (IMPORTANT GLEW after GLFW)
	##################################
	find_package(OpenGL REQUIRED)
	add_definitions(-DGLEW_STATIC)
	include_directories(${DEPENDENCIES_DIR}/glew/include)
	add_subdirectory(${DEPENDENCIES_DIR}/glew)
	list(APPEND LIBRARIES_2_LINK glew) 
endif()

###########################
# freetype2
###########################
# set(FREETYPE2_INCLUDE_DIR ${EXTERNALS_DIR}/freetype2/include)
# include_directories(${FREETYPE2_INCLUDE_DIR})
# add_subdirectory(${EXTERNALS_DIR}/freetype2)
# list(APPEND LIBRARIES_2_LINK freetype)

###########################
# SDL
###########################
if(USE_SDL AND NOT NACL AND NOT EMSCRIPTEN)
	include (CheckIncludeFiles)	
	add_definitions(-DSDL)
	set(SDL_INCLUDE_DIR ${DEPENDENCIES_DIR}/SDL/include)
	include_directories(${DEPENDENCIES_BINARY_DIR}/SDL/include ${SDL_INCLUDE_DIR})
	add_subdirectory(${DEPENDENCIES_DIR}/SDL)
    if (WIN32)
		if (MSVC)
		    find_package (Direct3D)
		    if (DIRECT3D_FOUND)
		        include_directories (${DIRECT3D_INCLUDE_DIRS})
		        list (APPEND LIBRARIES_2_LINK ${DIRECT3D_LIBRARIES} ${DIRECT3D_COMPILER_LIBRARIES})
		    endif ()
		endif ()
        list (APPEND LIBRARIES_2_LINK user32 gdi32 winmm imm32 ole32 oleaut32 version uuid )
    endif()

	###########################
	# SDL_mixer
	###########################
	add_subdirectory(${DEPENDENCIES_DIR}/SDL_mixer)
	include_directories(${DEPENDENCIES_DIR}/SDL_mixer)
	list(APPEND LIBRARIES_2_LINK SDL_mixer) # this shall go before SDL to avoid undefined SDL references
	list(APPEND LIBRARIES_2_LINK SDL)
endif()

if(EMSCRIPTEN)
	add_definitions(-DSDL)
endif()	

###########################
# stb
###########################
# add_subdirectory(${DEPENDENCIES_DIR}/stb)
# include_directories(${DEPENDENCIES_DIR}/stb)
# list(APPEND LIBRARIES_2_LINK stb) 

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
# glm
###########################
include_directories(${EXTERNALS_DIR}/glm)

###########################
# tools
###########################
if(NOT IS_TARGET_MOBILE AND NOT IS_TARGET_WEB)

	###########################
	###########################
	###########################
	# assimp
	###########################
	macro ( generate_memory_h)
		if(NOT EXISTS "${EXTERNALS_BYNARY_DIR}/assimp/memory.h")
			set (LOCAL_PROPERTIES_FILE ${EXTERNALS_BYNARY_DIR}/assimp/memory.h)
			file(WRITE ${LOCAL_PROPERTIES_FILE} "//Autogenerated by cmake\n")
			file(APPEND ${LOCAL_PROPERTIES_FILE} "#pragma once\n")
			file(APPEND ${LOCAL_PROPERTIES_FILE} "#include <memory>\n")
			file(APPEND ${LOCAL_PROPERTIES_FILE} "extern char* realpath(const char* in, char* out);\n")
		endif()
	endmacro( generate_memory_h)

	if(NACL)
		#include <memory.h> is not found on NACL
		generate_memory_h() # this solves the issue
		include_directories(${EXTERNALS_BYNARY_DIR}/assimp)
	endif(NACL)

	mark_as_disable_and_internal(BUILD_SHARED_LIB)
	mark_as_enable(ASSIMP_ENABLE_BOOST_WORKAROUND)
	mark_as_disable_and_internal(BUILD_SHARED_LIBS)
	mark_as_disable_and_internal(ASSIMP_BUILD_ASSIMP_TOOLS)
	mark_as_disable_and_internal(ASSIMP_BUILD_TESTS)
	mark_as_disable_and_internal(ASSIMP_BUILD_SAMPLES)
	include_directories(${EXTERNALS_DIR}/assimp/include)
	#add_subdirectory(${EXTERNALS_DIR}/assimp)
	###########################
	###########################
	###########################
	include (TemplateCompiler)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/NSG)
	SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/NSG)
	FOREACH(subdir ${SUBDIRS})
	   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/NSG/${subdir})
	ENDFOREACH()
	#add_subdirectory(tools/model_converter)
endif()

###########################
# nsg
###########################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/NSG)
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/NSG)
FOREACH(subdir ${SUBDIRS})
   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/NSG/${subdir})
ENDFOREACH()
add_subdirectory(NSG)

set(LIBRARIES_2_LINK NSG) #NSG will have all the dependencies already set 

###########################
# samples
###########################
set(SAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/samples)
SUBDIRLIST(SUBDIRS ${SAMPLES_DIR})
FOREACH(subdir ${SUBDIRS})
	IF(${BUILD_PROJECT} STREQUAL "all"  OR ${BUILD_PROJECT} STREQUAL "samples/${subdir}")
    	ADD_SUBDIRECTORY(${SAMPLES_DIR}/${subdir})
	ENDIF()
ENDFOREACH()

###########################
# tests
###########################
#if(NOT IS_TARGET_MOBILE AND NOT IS_TARGET_WEB)
	enable_testing()
	#include( CTest )
	set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
	SUBDIRLIST(SUBDIRS ${TEST_DIR})
	FOREACH(subdir ${SUBDIRS})
		IF(${BUILD_PROJECT} STREQUAL "all"  OR ${BUILD_PROJECT} STREQUAL "tests/${subdir}")
	   		ADD_SUBDIRECTORY(${TEST_DIR}/${subdir})
	   	ENDIF()
	ENDFOREACH()
#endif()




