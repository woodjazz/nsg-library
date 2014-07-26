PROJECT(NSG)

file(GLOB src "*.cpp" ${TINYXML2_INCLUDE_DIR}/tinyxml2.cpp)
file(GLOB hdr "*.h" ${TINYXML2_INCLUDE_DIR}/tinyxml2.h)

set (ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)

if(NOT IS_TARGET_WEB AND NOT IS_TARGET_MOBILE)
 	add_templates(1 GEN_OUT ${ASSETS_DIR}/fonts/anonymous_pro_regular_14_png.inl ${ASSETS_DIR}/fonts/anonymous_pro_regular_14.png)
 	list(APPEND assets ${ASSETS_DIR}/fonts/anonymous_pro_regular_14_png.inl)
 	add_templates(1 GEN_OUT ${ASSETS_DIR}/fonts/anonymous_pro_regular_14_xml.inl ${ASSETS_DIR}/fonts/anonymous_pro_regular_14.xml)
 	list(APPEND assets ${ASSETS_DIR}/fonts/anonymous_pro_regular_14_xml.inl)
endif()

include_directories(${ASSETS_DIR})

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
FOREACH(subdir ${SUBDIRS})
	file(GLOB tmp_src "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/*.cpp")
	file(GLOB tmp_hdr "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/*.h")
	set(src ${src} ${tmp_src})
	set(hdr ${hdr} ${tmp_hdr})
	source_group(${subdir} FILES ${tmp_hdr} ${tmp_src} ${assets})
ENDFOREACH()

if(ANDROID)
	# See http://developer.android.com/reference/android/app/NativeActivity.html
	list(APPEND hdr $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.h)
	list(APPEND src $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
	include_directories($ENV{ANDROID_NDK}/sources/android/native_app_glue)
elseif(IOS)

endif()
	
add_library(${PROJECT_NAME} STATIC ${src} ${hdr} ${assets})




