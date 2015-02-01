if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)
	
	###########################
	# blenderdna
	###########################
	set(DNA_GEN_TOOL blenderdna)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools/${DNA_GEN_TOOL})
	set(BLENDERDNA_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/tools/${DNA_GEN_TOOL}/${CMAKE_CFG_INTDIR}/${DNA_GEN_TOOL})

endif()

function(BLENDERDNA_TOOL input_file output_dir)

	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)

	    get_filename_component(INPUTEXTENSION ${input_file} EXT)

	    if(${INPUTEXTENSION} MATCHES ".blend")

		    get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
		    get_filename_component(INPUTNAME ${input_file} NAME)
		    get_filename_component(OUTPUTDIR ${output_dir} ABSOLUTE)

		    #string(REGEX REPLACE "[.][^.]+$" ".xml" OUTPUTNAME ${INPUTNAME}) # replace extension by xml
		    # message("${INPUTFILE}")
		    # message("${INPUTNAME}")
		    # message("${OUTPUTNAME}")  
	    
		    set(BLENDERDNA_CMD ${BLENDERDNA_EXECUTABLE} -i ${INPUTFILE} -o ${OUTPUTDIR} ${ARGN})

		    #message("${BLENDERDNA_CMD}")

		    set(TARGET_NAME GENERATE_DNA_FOR_${INPUTNAME})

		    add_custom_command(OUTPUT ${OUTPUTDIR}/Blender.h
  				COMMAND ${BLENDERDNA_CMD}
  				DEPENDS ${INPUTFILE}
			)

			add_custom_target(${TARGET_NAME} ALL
   				DEPENDS ${OUTPUTDIR}/Blender.h
			)

			add_dependencies(${TARGET_NAME} ${DNA_GEN_TOOL})
			add_dependencies(converter ${TARGET_NAME})

		endif()

	endif()

endfunction(BLENDERDNA_TOOL)

