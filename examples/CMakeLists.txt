PROJECT(examples)

file(GLOB src "*.cpp")

file(GLOB hdr "*.h")

add_executable(${PROJECT_NAME} ${src} ${hdr} )

target_link_libraries(${PROJECT_NAME} nsg)
