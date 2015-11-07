function(EXPORT_BLEND_TOOL input_file output_dir)

	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE AND NOT COVERITY)

		if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
		  set(BLENDER_EXECUTABLE "/Applications/Blender/blender.app/Contents/MacOS/blender")
		elseif(WIN32)
		  set(BLENDER_EXECUTABLE "D:/tmp/Blender-2.75-windows64/blender.exe")
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

endfunction(EXPORT_BLEND_TOOL)

