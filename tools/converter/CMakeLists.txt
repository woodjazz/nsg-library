##################################
# blenderserialize
##################################
include_directories(blenderserialize)
add_subdirectory(blenderserialize)
list(APPEND LIBRARIES_2_LINK blenderserialize)

###########################
# assimpconverter
###########################

include_directories(assimpconverter)
add_subdirectory(assimpconverter)
list(APPEND LIBRARIES_2_LINK assimpconverter)

###########################
# blenderconverter
###########################

include_directories(blenderconverter)
add_subdirectory(blenderconverter)
list(APPEND LIBRARIES_2_LINK blenderconverter)

###########################
# converter
###########################
list(APPEND LIBRARIES_2_LINK assimp)

setup_tool()

list(REMOVE_ITEM LIBRARIES_2_LINK assimp)
list(REMOVE_ITEM LIBRARIES_2_LINK assimpconverter)
list(REMOVE_ITEM LIBRARIES_2_LINK bulletfileloader)
list(REMOVE_ITEM LIBRARIES_2_LINK blenderserialize)


