# ----------------------------------------------------------------------------#
#                                                                             #
#  Compile time template generator                                            #
#   Compile N source files to C strings                                       #
#   Usage: ${TARGET} ${OUTFILE} ${SOURCE0} ... ${SOURCE[N]}                   #
#                                                                             #
# ----------------------------------------------------------------------------#
subdirs(${CMAKE_CURRENT_SOURCE_DIR}/tools/tcl)
set(TCL_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/tcl/${CMAKE_CFG_INTDIR}/tcl)
# ----------------------------------------------------------------------------#
#                                                                             #
# Get a list of absolute paths                                                #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(TMPL_ABSOLUTE_SRC OUT)
    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        set(${OUT} ${${OUT}} ${N})
    endforeach(it)
endmacro(TMPL_ABSOLUTE_SRC)
# ----------------------------------------------------------------------------#
#                                                                             #
#   Usage: ${TCL_EXECUTABLE} ${OUTFILE} {ARGN}                                #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(ADD_TEMPLATES BINARY OUT GENERATED)
		
    get_filename_component(OUTFILE ${GENERATED} ABSOLUTE)
    TMPL_ABSOLUTE_SRC(TEMPLATES ${ARGN})
    get_filename_component(ONAME ${OUTFILE} NAME)
	
	if (${BINARY})
		set(TCL_CMD ${TCL_EXECUTABLE} -b ${OUTFILE} ${TEMPLATES})
	else()
		set(TCL_CMD ${TCL_EXECUTABLE} ${OUTFILE} ${TEMPLATES})
	endif()	
	
	if(NOT IS_TARGET_MOBILE AND NOT IS_TARGET_WEB)
		add_custom_command(
	    	OUTPUT ${OUTFILE}
	    	COMMAND ${TCL_CMD}
	    	DEPENDS ${TCL_EXECUTABLE} ${TEMPLATES}
	    	COMMENT "Building ${ONAME}:${TCL_CMD}"
	    )
	else()
		message("${CMAKE_CURRENT_SOURCE_DIR}/tools/tcl")
		message("If needed (Build): ${ONAME}:${TCL_CMD}")
	endif()

	set(${OUT} ${OUTFILE})
	set(TEMPLATES)
	
endmacro(ADD_TEMPLATES)
