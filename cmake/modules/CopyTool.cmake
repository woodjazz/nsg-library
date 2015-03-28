function(COPY_TOOL input_file output_dir)

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

endfunction(COPY_TOOL)