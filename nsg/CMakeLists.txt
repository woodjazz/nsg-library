PROJECT(nsg)

file(GLOB src "*.cpp")

file(GLOB hdr "*.h")
	
add_library(${PROJECT_NAME} STATIC ${src} ${hdr})




