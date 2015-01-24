macro(COPY_TOOL input_file output_dir)

	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)

		get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
		get_filename_component(INPUTNAME ${input_file} NAME)
		get_filename_component(OUTPUTDIR ${output_dir} ABSOLUTE)

	    add_custom_target(${INPUTNAME} 
	            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${INPUTFILE} ${OUTPUTDIR}/${INPUTNAME}
	            DEPENDS ${INPUTFILE}
	            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

	    add_dependencies(${PROJECT_NAME} ${INPUTNAME})

	endif()

endmacro(COPY_TOOL)