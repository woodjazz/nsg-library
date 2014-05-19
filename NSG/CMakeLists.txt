PROJECT(NSG)

file(GLOB src "*.cpp")
file(GLOB hdr "*.h")

if(ANDROID)
	# See http://developer.android.com/reference/android/app/NativeActivity.html
	list(APPEND hdr $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.h)
	list(APPEND src $ENV{ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
	include_directories($ENV{ANDROID_NDK}/sources/android/native_app_glue)
elseif(IOS)

endif()
	
add_library(${PROJECT_NAME} STATIC ${src}  ${hdr})




