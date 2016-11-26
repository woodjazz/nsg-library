cmake_minimum_required(VERSION 2.8)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

PROJECT(nsg-library)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPENDENCIES_DIR ${ROOT_SOURCE_DIR}/dependencies)
set(EMSCRIPTEN_DIR ${ROOT_SOURCE_DIR}/emscripten)
set(EXTERNALS_DIR ${ROOT_SOURCE_DIR}/externals)
set(TOOLS_DIR ${ROOT_SOURCE_DIR}/tools)
set(CONVERTER_TOOL_SOURCE_DIR ${TOOLS_DIR}/blenderserialize)
set(BLENDERDNA_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/tools/blenderdna/${CMAKE_CFG_INTDIR}/blenderdna)
set(CONVERT_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/tools/converter/${CMAKE_CFG_INTDIR}/converter)

set(ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(DEPENDENCIES_BINARY_DIR ${ROOT_BINARY_DIR}/dependencies)
set(EXTERNALS_BYNARY_DIR ${ROOT_BINARY_DIR}/externals)
set(LIBRARIES_FOLDER_NAME "LIBRARIES")

set (CMAKE_MODULE_PATH ${ROOT_SOURCE_DIR}/cmake/modules) # Set CMake modules search path
include (Common)
include (Utils) 
set(NEEDS_X11 1)
setup_common() #has to be called before anything

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

###########################
# CLEAN_SAMPLES_DATA
###########################
add_custom_target(CLEAN_SAMPLES_DATA
   COMMAND ${CMAKE_COMMAND} -P ${CMAKE_MODULE_PATH}/CleanSamplesData.cmake
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   COMMENT "Deleting all samples data files"
)

###########################
# CLEAN_TEST_DATA
###########################
add_custom_target(CLEAN_TEST_DATA
   COMMAND ${CMAKE_COMMAND} -P ${CMAKE_MODULE_PATH}/CleanTestData.cmake
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   COMMENT "Deleting all tests data files"
)

###########################
# BUILD_DATA_FILES
###########################
add_custom_target(BUILD_DATA_FILES
   COMMAND ${CMAKE_COMMAND} -P ${CMAKE_MODULE_PATH}/BuildDataFiles.cmake
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   COMMENT "Building data files"
)

##################################
# LZ4
##################################
include_directories(${DEPENDENCIES_DIR}/LZ4)
add_subdirectory(${DEPENDENCIES_DIR}/LZ4)
list(APPEND LIBRARIES_2_LINK LZ4)
set_property(TARGET LZ4 PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})

##################################
# bullet
##################################
#add_definitions(-DBT_USE_DOUBLE_PRECISION)
include_directories(${EXTERNALS_DIR}/bullet/src)
add_subdirectory(${EXTERNALS_DIR})
list(APPEND LIBRARIES_2_LINK bulletCollision)
list(APPEND LIBRARIES_2_LINK bulletDynamics)
list(APPEND LIBRARIES_2_LINK bulletSoftBody)
list(APPEND LIBRARIES_2_LINK bulletLinearMath)
list(APPEND LIBRARIES_2_LINK bulletDynamics)
list(APPEND LIBRARIES_2_LINK bulletCollision)
set_property(TARGET bulletCollision PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})
set_property(TARGET bulletDynamics PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})
set_property(TARGET bulletSoftBody PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})
set_property(TARGET bulletLinearMath PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})

###########################
# externals
###########################
#add_subdirectory(${EXTERNALS_DIR})

###########################
# pugixml
###########################
set(PUGIXML_INCLUDE_DIR ${EXTERNALS_DIR}/pugixml/src)
include_directories(${PUGIXML_INCLUDE_DIR})
add_definitions(-DPUGIXML_NO_EXCEPTIONS)
list(APPEND LIBRARIES_2_LINK pugixml)
set_property(TARGET pugixml PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})

###########################
# imgui
###########################
set(IMGUI_INCLUDE_DIR ${EXTERNALS_DIR}/imgui)
include_directories(${IMGUI_INCLUDE_DIR})
list(APPEND LIBRARIES_2_LINK imgui)
set_property(TARGET imgui PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})

##################################
# opengl
##################################
if(NOT IS_TARGET_MOBILE AND NOT IS_TARGET_WEB AND NOT IS_TARGET_OSX)
	find_package(OpenGL REQUIRED)
	list(APPEND LIBRARIES_2_LINK ${OPENGL_LIBRARY})
endif()

##################################
# jpeg
##################################
include_directories(${DEPENDENCIES_DIR}/jpeg)
add_subdirectory(${DEPENDENCIES_DIR}/jpeg)
list(APPEND LIBRARIES_2_LINK jpeg)
set_property(TARGET jpeg PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})

##################################
# libb64
##################################
include_directories(${DEPENDENCIES_DIR}/libb64/include/)
add_subdirectory(${DEPENDENCIES_DIR}/libb64)
list(APPEND LIBRARIES_2_LINK libb64)
set_property(TARGET libb64 PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})

##################################
# StanHull
##################################
include_directories(${DEPENDENCIES_DIR}/StanHull)
add_subdirectory(${DEPENDENCIES_DIR}/StanHull)
list(APPEND LIBRARIES_2_LINK StanHull) 
set_property(TARGET StanHull PROPERTY FOLDER ${LIBRARIES_FOLDER_NAME})

###########################
# stb
###########################
include_directories(${EXTERNALS_DIR}/stb)

###########################
# tclap
###########################
include_directories(${DEPENDENCIES_DIR}/tclap/include)

###########################
# NSG
###########################
include_directories(${ROOT_SOURCE_DIR}/NSG)
subdirlist(SUBDIRS ${ROOT_SOURCE_DIR}/NSG)
FOREACH(subdir ${SUBDIRS})
   include_directories(${ROOT_SOURCE_DIR}/NSG/${subdir})
ENDFOREACH()
add_subdirectory(NSG)
set(LIBRARIES_2_LINK NSG) #reset libraries to link: NSG will have all the dependencies already set 

###########################
# tools
###########################
if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)
	##################################
	# common
	##################################
	include_directories(${TOOLS_DIR}/common)
	add_subdirectory(${TOOLS_DIR}/common)
	set(LIBRARIES_2_LINK common NSG) #reset libraries to link: common will have all the dependencies already set 
	
	###########################
	# converter
	###########################
	add_subdirectory(${TOOLS_DIR}/converter)
	set(LIBRARIES_2_LINK NSG) #reset libraries to link: NSG will have all the dependencies already set 
	#list(REMOVE_ITEM LIBRARIES_2_LINK common)

endif()

###########################
# samples
###########################
include (ConvertTool) 
include (ExportBlendTool)
include (CopyTool)
set(SAMPLES_DIR ${ROOT_SOURCE_DIR}/samples)
subdirlist(SUBDIRS ${SAMPLES_DIR})
foreach(subdir ${SUBDIRS})
	add_subdirectory(${SAMPLES_DIR}/${subdir})
endforeach()

###########################
# tests
###########################
if(NOT IS_TARGET_MOBILE AND NOT IS_TARGET_WEB)
	enable_testing()
	#include( CTest )
	set(TEST_DIR ${ROOT_SOURCE_DIR}/tests)
	subdirlist(SUBDIRS ${TEST_DIR})
	foreach(subdir ${SUBDIRS})
		add_subdirectory(${TEST_DIR}/${subdir})
	endforeach()
endif()

