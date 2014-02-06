PROJECT(test)

file(GLOB src "*.cpp")

file(GLOB hdr "*.h")

add_library(${PROJECT_NAME} STATIC ${src} ${hdr})

#set_source_files_properties(App.cpp PROPERTIES COMPILE_FLAGS "-o ${CMAKE_CURRENT_SOURCE_DIR}/../App.cpp.obj")
#set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(data_dir ${CMAKE_CURRENT_SOURCE_DIR}/../Data)

add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/..
              WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
