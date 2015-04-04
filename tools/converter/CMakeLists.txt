include_directories(${CMAKE_CURRENT_SOURCE_DIR})

##################################
# blenderserialize
##################################
include_directories(blenderserialize)
add_subdirectory(blenderserialize)
list(APPEND LIBRARIES_2_LINK blenderserialize)

###########################
# blenderconverter
###########################

include_directories(blenderconverter)
add_subdirectory(blenderconverter)
list(APPEND LIBRARIES_2_LINK blenderconverter)

###########################
# converter
###########################
setup_tool()

list(REMOVE_ITEM LIBRARIES_2_LINK blenderconverter)
list(REMOVE_ITEM LIBRARIES_2_LINK blenderserialize)


