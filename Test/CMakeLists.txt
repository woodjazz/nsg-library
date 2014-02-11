PROJECT(test)

file(GLOB src "*.cpp")

file(GLOB hdr "*.h")

add_executable(${PROJECT_NAME} ${src} ${hdr} )

if(NACL)
	target_link_libraries(${PROJECT_NAME} nsg soil ppapi_cpp ppapi ppapi_gles2 pthread nacl_io)
elseif(ANDROID)
	target_link_libraries(${PROJECT_NAME} nsg soil GLESv2 log android EGL)
else()
	target_link_libraries(${PROJECT_NAME} nsg glew soil glfw)
endif()

set(data_dir ${CMAKE_CURRENT_SOURCE_DIR}/Data)

add_custom_command(
 	TARGET ${PROJECT_NAME} POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

if(NACL)
	
	pnacl_finalise(test)

	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
      	COMMAND python
      	ARGS $ENV{NACL_SDK_ROOT}/tools/httpd.py --no_dir_check -C ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()
