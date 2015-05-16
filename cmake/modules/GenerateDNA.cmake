function(GENERATE_DNA_TOOL input_file)

	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)

		get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
		get_filename_component(INPUTNAME ${input_file} NAME)

		set(BLENDERDNA_CMD ${BLENDERDNA_EXECUTABLE} -i ${INPUTFILE} -o ${CONVERTER_TOOL_SOURCE_DIR})

		set(OUTPUTFILES Blender.h BlenderDNA.cpp)

    	add_custom_command(OUTPUT ${OUTPUTFILES}
			COMMAND ${BLENDERDNA_CMD}
			DEPENDS ${BLENDERDNA_EXECUTABLE} 
			COMMENT "Generating DNA ${OUTPUTFILES}"
		)

	    set(TARGET_NAME DNA_GEN)

		add_custom_target(${TARGET_NAME} ALL
   			DEPENDS ${OUTPUTFILES} 
		)

		set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "tools")

	endif()

endfunction(GENERATE_DNA_TOOL)