macro(COPY_TOOL input_file)

	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)

		get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
		get_filename_component(INPUTNAME ${input_file} NAME)

	    add_custom_target(${INPUTNAME} ALL
	            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${INPUTFILE} ${CMAKE_CURRENT_SOURCE_DIR}/data/${INPUTNAME}
	            DEPENDS ${INPUTFILE}
	            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

	    add_dependencies(${PROJECT_NAME} ${INPUTNAME})

	endif()

endmacro(COPY_TOOL)