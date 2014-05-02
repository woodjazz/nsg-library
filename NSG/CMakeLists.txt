PROJECT(nsg)

file(GLOB src "*.cpp")

file(GLOB hdr "*.h")

if(ANDROID)
	list(APPEND hdr $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.h)
	list(APPEND src $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
	include_directories($ENV{ANDROID_NDK}/sources/android/native_app_glue)
endif()
	
add_library(${PROJECT_NAME} STATIC ${src} ${hdr})

if(NACL)
	target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK} ppapi_cpp ppapi ppapi_gles2 pthread nacl_io)
elseif(ANDROID)
	target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK} GLESv2 log android EGL)
else()
	if(UNIX AND NOT APPLE)
		target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK} ${X11_Xxf86vm_LIB} ${X11_Xrandr_LIB} ${X11_Xinput_LIB})
	elseif(WIN32)
		target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})		
	else()
		target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})		
	endif()
endif()




