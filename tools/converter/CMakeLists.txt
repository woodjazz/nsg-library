###########################
# assimpconverter
###########################

include_directories(assimpconverter)
add_subdirectory(assimpconverter)
list(APPEND LIBRARIES_2_LINK assimpconverter)

###########################
# converter
###########################
list(APPEND LIBRARIES_2_LINK assimp)

setup_tool()

list(REMOVE_ITEM LIBRARIES_2_LINK assimp)
list(REMOVE_ITEM LIBRARIES_2_LINK assimpconverter)


