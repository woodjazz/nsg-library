PROJECT(test)

file(GLOB src "*.cpp")

file(GLOB hdr "*.h")

add_library(${PROJECT_NAME} STATIC ${src} ${hdr})

set(data_dir ${CMAKE_CURRENT_SOURCE_DIR}/../Data)

add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/..
              WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
