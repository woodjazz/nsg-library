function(CONVERT_TOOL input_file output_dir)

	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)

		BLENDERDNA_TOOL(${input_file} ${CONVERTER_TOOL_SOURCE_DIR})

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

		add_custom_target(${TARGET_NAME} ALL
   			DEPENDS ${OUTPUT_FILE} 
		)

		set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "CONVERTERS")

		add_dependencies(${TARGET_NAME} converter)
        add_dependencies(${DATA_TARGET} ${TARGET_NAME})

	endif()

endfunction(CONVERT_TOOL)

