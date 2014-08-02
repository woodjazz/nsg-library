PROJECT(glew)

file(GLOB src "src/glew.c")
file(GLOB hdr "include/GL/*.h")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # If on Xcode, rename files to .mm
	file(COPY ${src} DESTINATION ${CMAKE_BINARY_DIR}/external/GLEW-ObjectiveC/)
	file(RENAME ${CMAKE_BINARY_DIR}/external/GLEW-ObjectiveC/glew.c ${CMAKE_BINARY_DIR}/external/GLEW-ObjectiveC/glew.m)
	set(src ${CMAKE_BINARY_DIR}/external/GLEW-ObjectiveC/glew.m)
	add_definitions(-D_MACOSX -ObjC++)
	set(EXTRA_LIBS "-framework Cocoa -framework OpenGL -framework IOKit -framework Foundation -framework AppKit")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions(-D_UNIX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-D_WINDOWS)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

setup_library()

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} ${EXTRA_LIBS})




