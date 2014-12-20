if(EMSCRIPTEN)	
	PROJECT(NSGWorker)
	file(GLOB src "*.cpp")
	file(GLOB hdr "*.h")
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/NSGWorker.cpp PROPERTIES COMPILE_FLAGS "-s BUILD_AS_WORKER=1")
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/NSGWorker.cpp PROPERTIES COMPILE_FLAGS "-s EXPORTED_FUNCTIONS=['NSGWorkerEntryPoint']")
	setup_executable()
	#target_link_libraries(${PROJECT_NAME} NSG)
endif()


