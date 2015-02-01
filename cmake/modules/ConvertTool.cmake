if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)
	
	###########################
	# assimp
	###########################
	mark_as_disable_and_internal(BUILD_SHARED_LIB)
	mark_as_enable(ASSIMP_ENABLE_BOOST_WORKAROUND)
	mark_as_disable_and_internal(BUILD_SHARED_LIBS)
	mark_as_disable_and_internal(ASSIMP_BUILD_ASSIMP_TOOLS)
	mark_as_disable_and_internal(ASSIMP_BUILD_TESTS)
	mark_as_disable_and_internal(ASSIMP_BUILD_SAMPLES)
	include_directories(${EXTERNALS_DIR}/assimp/include)
	add_subdirectory(${EXTERNALS_DIR}/assimp)

	###########################
	# converter
	###########################
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools/converter)
	set(CONVERT_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/tools/converter/${CMAKE_CFG_INTDIR}/converter)

endif()

function(CONVERT_TOOL input_file output_dir)

	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)

		BLENDERDNA_TOOL(${input_file} ${CONVERTER_TOOL_SOURCE_DIR})

	    get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
	    get_filename_component(INPUTNAME ${input_file} NAME)
	    get_filename_component(INPUTEXTENSION ${input_file} EXT)
	    get_filename_component(OUTPUTDIR ${output_dir} ABSOLUTE)
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

	    set(TARGET_NAME CONVERT_${INPUTNAME})

		add_custom_target(${TARGET_NAME} ALL
   			DEPENDS ${OUTPUT_FILE}
		)

		add_dependencies(${TARGET_NAME} converter)
        add_dependencies(${DATA_TARGET} ${TARGET_NAME})

	endif()

endfunction(CONVERT_TOOL)

