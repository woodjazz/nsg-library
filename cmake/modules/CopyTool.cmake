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
function(copy_tool input_file output_dir)
	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)
		get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
		get_filename_component(INPUTNAME ${input_file} NAME)
		get_filename_component(OUTPUTDIR ${output_dir} ABSOLUTE)
	    add_custom_command(OUTPUT ${OUTPUTDIR}/${INPUTNAME}
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${INPUTFILE} ${OUTPUTDIR}/${INPUTNAME}
			DEPENDS ${INPUTFILE}
		)
		set(TARGET_NAME COPY_FILE_${PROJECT_NAME}${INPUTNAME})
		add_custom_target(${TARGET_NAME} ALL
			DEPENDS ${OUTPUTDIR}/${INPUTNAME}
		)
		set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "COPIES")
	    add_dependencies(${PROJECT_NAME} ${TARGET_NAME})
	endif()
endfunction()