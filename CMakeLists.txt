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

if(WIN32)

	if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
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

if(CMAKE_COMPILER_IS_GNUCXX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
endif()	

if(APPLE)
	set(CMAKE_CXX_FLAGS "-std=gnu++11 -stdlib=libc++")
endif()

macro ( mark_as_disable_and_internal _var )
	set ( ${_var} OFF CACHE INTERNAL "disable and hide this!" FORCE )
endmacro( mark_as_disable_and_internal _var )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(
	-DGLEW_STATIC
)

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if(NOT NACL AND NOT ANDROID)

	find_package(OpenGL REQUIRED)
	###########################
	# glew
	###########################

	set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
	include_directories(${GLEW_INCLUDE_DIR})
	add_subdirectory(glew)

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

###########################
# freetype2
###########################
set(FREETYPE2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/freetype2/include)
include_directories(${FREETYPE2_INCLUDE_DIR})
add_subdirectory(freetype2)

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




