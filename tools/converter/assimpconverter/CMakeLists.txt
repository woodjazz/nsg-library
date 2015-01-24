get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
PROJECT(${PROJECT_NAME})

file(GLOB src "*.cpp")
file(GLOB hdr "*.h")
setup_library()
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tools")
target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})

