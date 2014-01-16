# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.8)

PROJECT(nsg-library)

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
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()	

###########################
# nsg
###########################
set(NSG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nsg)
include_directories(${NSG_INCLUDE_DIR})
add_subdirectory(nsg)

###########################
# test
###########################
add_subdirectory(test)

###########################
# examples
###########################
add_subdirectory(examples)
