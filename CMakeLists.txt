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

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules) # Set CMake modules search path
include (Common) # Include NSG cmake common module
setup_common() #has to be called before anything

set(USE_SDL ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPENDENCIES_DIR ${ROOT_SOURCE_DIR}/dependencies)
set(EXTERNALS_DIR ${ROOT_SOURCE_DIR}/externals)

set(ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(DEPENDENCIES_BINARY_DIR ${ROOT_BINARY_DIR}/dependencies)
set(EXTERNALS_BYNARY_DIR ${ROOT_BINARY_DIR}/externals)

###########################
# pugixml
###########################
set(PUGIXML_INCLUDE_DIR ${EXTERNALS_DIR}/pugixml/src)
include_directories(${PUGIXML_INCLUDE_DIR})
add_subdirectory(${EXTERNALS_DIR})
list(APPEND LIBRARIES_2_LINK pugixml)

##################################
# glew
##################################
if(NOT IS_TARGET_MOBILE AND NOT IS_TARGET_WEB)
	find_package(OpenGL REQUIRED)
	add_definitions(-DGLEW_STATIC)
	include_directories(${DEPENDENCIES_DIR}/glew/include)
	add_subdirectory(${DEPENDENCIES_DIR}/glew)
	list(APPEND LIBRARIES_2_LINK glew) 
endif()

##################################
# jpeg
##################################
include_directories(${DEPENDENCIES_DIR}/jpeg)
add_subdirectory(${DEPENDENCIES_DIR}/jpeg)
list(APPEND LIBRARIES_2_LINK jpeg) 

##################################
# StanHull
##################################
include_directories(${DEPENDENCIES_DIR}/StanHull)
add_subdirectory(${DEPENDENCIES_DIR}/StanHull)
list(APPEND LIBRARIES_2_LINK StanHull) 

##################################
# bullet
##################################
include_directories(${DEPENDENCIES_DIR}/bullet/src)
add_subdirectory(${DEPENDENCIES_DIR}/bullet)
list(APPEND LIBRARIES_2_LINK bulletCollision)
list(APPEND LIBRARIES_2_LINK bulletDynamics)
list(APPEND LIBRARIES_2_LINK bulletSoftBody)
list(APPEND LIBRARIES_2_LINK bulletLinearMath)

###########################
# SDL
###########################
add_definitions(-DSDL)
if(NOT EMSCRIPTEN) #EMSCRIPTEN supports/includes SDL 1.2
	include (CheckIncludeFiles)	
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

###########################
# stb
###########################
include_directories(${EXTERNALS_DIR}/stb)

###########################
# glm
###########################
include_directories(${EXTERNALS_DIR}/glm)

###########################
# tclap
###########################
include_directories(${EXTERNALS_DIR}/tclap/include)

###########################
# NSG
###########################
if(NOT IS_TARGET_MOBILE AND NOT IS_TARGET_WEB)
	include (TemplateCompiler) # tcl tool
endif()
include_directories(${ROOT_SOURCE_DIR}/NSG)
SUBDIRLIST(SUBDIRS ${ROOT_SOURCE_DIR}/NSG)
FOREACH(subdir ${SUBDIRS})
   include_directories(${ROOT_SOURCE_DIR}/NSG/${subdir})
ENDFOREACH()
add_subdirectory(NSG)
set(LIBRARIES_2_LINK NSG) #reset libraries to links since NSG will have all the dependencies already set 

##################################
# NSGWorker
##################################
if(EMSCRIPTEN)
	include_directories(${ROOT_SOURCE_DIR}/NSGWorker)
	add_subdirectory(${ROOT_SOURCE_DIR}/NSGWorker)
endif()

###########################
# samples
###########################
include (ConvertTool) # convert tool
set(SAMPLES_DIR ${ROOT_SOURCE_DIR}/samples)
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
	set(TEST_DIR ${ROOT_SOURCE_DIR}/tests)
	SUBDIRLIST(SUBDIRS ${TEST_DIR})
	FOREACH(subdir ${SUBDIRS})
		IF(${BUILD_PROJECT} STREQUAL "all"  OR ${BUILD_PROJECT} STREQUAL "tests/${subdir}")
	   		ADD_SUBDIRECTORY(${TEST_DIR}/${subdir})
	   	ENDIF()
	ENDFOREACH()
#endif()

