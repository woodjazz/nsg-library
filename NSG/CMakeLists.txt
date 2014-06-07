PROJECT(NSG)

file(GLOB src "*.cpp")
file(GLOB hdr "*.h")

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
FOREACH(subdir ${SUBDIRS})
	file(GLOB tmp_src "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/*.cpp")
	file(GLOB tmp_hdr "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/*.h")
	set(src ${src} ${tmp_src})
	set(hdr ${hdr} ${tmp_hdr})
	source_group(${subdir} FILES ${tmp_hdr} ${tmp_src})
#	source_group(${subdir}\\B FILES ${tmp_src})
ENDFOREACH()


if(ANDROID)
	# See http://developer.android.com/reference/android/app/NativeActivity.html
	list(APPEND hdr $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.h)
	list(APPEND src $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
	include_directories($ENV{ANDROID_NDK}/sources/android/native_app_glue)
elseif(IOS)

endif()
	
add_library(${PROJECT_NAME} STATIC ${src}  ${hdr})




