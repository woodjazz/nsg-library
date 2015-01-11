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

macro(CONVERT_TOOL input_file)

	if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)

	    get_filename_component(INPUTFILE ${input_file} ABSOLUTE)
	    get_filename_component(INPUTNAME ${input_file} NAME)
	    #string(REGEX REPLACE "[.][^.]+$" ".xml" OUTPUTNAME ${INPUTNAME}) # replace extension by xml
	    # message("${INPUTFILE}")
	    # message("${INPUTNAME}")
	    # message("${OUTPUTNAME}")  
	    set(OUTPUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/data/)
	    
	    set(CONVERT_CMD ${CONVERT_EXECUTABLE} -i ${INPUTFILE} -o ${OUTPUTDIR} ${ARGN})

	    #message("${CONVERT_CMD}")

	    add_custom_target(${INPUTNAME} ALL
	            COMMAND ${CONVERT_CMD}
	            DEPENDS ${CONVERT_EXECUTABLE} ${INPUTFILE}
	            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

		add_dependencies(${INPUTNAME} converter)
	    add_dependencies(${PROJECT_NAME} ${INPUTNAME})

	    # add_custom_command(
	    #     TARGET ${PROJECT_NAME} POST_BUILD
	    #         COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OUTPUTFILE} ${CMAKE_CURRENT_BINARY_DIR}/data/${OUTPUTNAME}
	    #         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

	endif()

endmacro(CONVERT_TOOL)

