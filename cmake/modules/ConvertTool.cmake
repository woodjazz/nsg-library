# /*
# -------------------------------------------------------------------------------
# This file is part of nsg-library.
# http://github.com/woodjazz/nsg-library

# Copyright (c) 2014-2016 Néstor Silveira Gorski

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
function(convert_tool input_file output_dir)
	if(NOT CMAKE_CROSSCOMPILING AND NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)
	    get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
	    get_filename_component(INPUTNAME ${input_file} NAME)
	    get_filename_component(INPUTEXTENSION ${input_file} EXT)
	    get_filename_component(OUTPUTDIR ${output_dir} ABSOLUTE)
		if(NOT(EXISTS ${OUTPUTDIR}))
			file(MAKE_DIRECTORY ${OUTPUTDIR})
		endif()			
	    string(REGEX REPLACE "[.][^.]+$" ".xml" OUTPUTNAME ${INPUTNAME}) # replace extension by xml
	    # message("${INPUTFILE}")
	    # message("${INPUTNAME}")
	    # message("${OUTPUTNAME}")  
	    # message("${INPUTEXTENSION}")  
	    set(CONVERT_CMD ${CONVERT_EXECUTABLE} -i ${INPUTFILE} -o ${OUTPUTDIR} ${ARGN})
	    #message("${CONVERT_CMD}")
	    if(${INPUTEXTENSION} MATCHES ".blend")
	    	set(OUTPUT_FILE ${OUTPUTDIR}/b${OUTPUTNAME})
	    else()
	    	set(OUTPUT_FILE ${OUTPUTDIR}/${OUTPUTNAME})
	    endif()
    	add_custom_command(OUTPUT ${OUTPUT_FILE}
			COMMAND ${CONVERT_CMD}
			DEPENDS ${INPUTFILE} 
			COMMENT "Generating ${OUTPUT_FILE}"
		)
	    set(TARGET_NAME CONVERT_${PROJECT_NAME}_${INPUTNAME})
		add_custom_target(${TARGET_NAME} 
   			DEPENDS ${OUTPUT_FILE} ${INPUTFILE}
		)
		set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "CONVERTERS")
		add_dependencies(${TARGET_NAME} converter)
        add_dependencies(${DATA_TARGET} ${TARGET_NAME})
	endif()
endfunction()

