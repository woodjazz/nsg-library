PROJECT(soil)

file(GLOB src "*.c")

file(GLOB hdr "*.h")
	
add_library(${PROJECT_NAME} STATIC ${src} ${hdr})




