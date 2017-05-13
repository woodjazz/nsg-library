# /*
# -------------------------------------------------------------------------------
# This file is part of nsg-library.
# http://github.com/woodjazz/nsg-library

# Copyright (c) 2014-2017 Néstor Silveira Gorski

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
function(export_blend_tool input_file output_dir)
	if(NOT CMAKE_CROSSCOMPILING AND NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE AND NOT COVERITY)
        find_program(BLENDER_EXECUTABLE blender)
        if(${BLENDER_EXECUTABLE} MATCHES "NOTFOUND")
    		if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
	    		set(BLENDER_EXECUTABLE "$ENV{BLENDER_BIN}/blender.app/Contents/MacOS/blender")
    		elseif(WIN32)
	    		set(BLENDER_EXECUTABLE "$ENV{BLENDER_BIN}/blender.exe")
    		else()
            	set(BLENDER_EXECUTABLE "$ENV{BLENDER_BIN}/blender")
    		endif() 
        else()
            #message("Blender has been found at ${BLENDER_EXECUTABLE}")
        endif()
		if(EXISTS ${BLENDER_EXECUTABLE})
			set(EXPORT_SCRIPT ${ROOT_SOURCE_DIR}/tools/blenderexport/Export.py)
		    get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
		    get_filename_component(INPUTNAME ${input_file} NAME)
		    get_filename_component(INPUTEXTENSION ${input_file} EXT)
		    get_filename_component(OUTPUTDIR ${output_dir} ABSOLUTE)
			if(NOT(EXISTS ${OUTPUTDIR}))
				file(MAKE_DIRECTORY ${OUTPUTDIR})
			endif()			
		    string(REGEX REPLACE "[.][^.]+$" ".xml" OUTPUTNAME ${INPUTNAME}) # replace extension by xml
		    #message("${INPUTFILE}")
		    #message("${INPUTNAME}")
		    #message("${OUTPUTNAME}")  
		    #message("${OUTPUTDIR}")  
		    #message("${INPUTEXTENSION}")  
			set(OUTPUT_FILE ${OUTPUTDIR}/${OUTPUTNAME})
		    set(EXPORT_CMD ${BLENDER_EXECUTABLE} ${INPUTFILE} --background --python ${EXPORT_SCRIPT} -- ${OUTPUTDIR})
	    	add_custom_command(OUTPUT ${OUTPUT_FILE}
				COMMAND ${EXPORT_CMD}
				DEPENDS ${INPUTFILE} 
				COMMENT "Exporting ${INPUTFILE}"
			)
			string(REGEX REPLACE "[/:.\\-]" "_" OUT_DIR_CONVERTED ${output_dir})
		    set(TARGET_NAME EXPORT_${PROJECT_NAME}_${INPUTNAME}_${OUT_DIR_CONVERTED})
			add_custom_target(${TARGET_NAME}
	   			DEPENDS ${OUTPUT_FILE} ${INPUTFILE}
			)
			set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "EXPORTS")
			add_dependencies(${DATA_TARGET} ${TARGET_NAME})
		else()
			message(WARNING, "Blender not found. Cannot export blend files!!!")
		endif()
	endif()
endfunction()

