
##################################################################################
##################################################################################
##################################################################################
macro (setup_common)
    if (CMAKE_GENERATOR STREQUAL Xcode)
       set (XCODE TRUE)
    endif ()

    if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
        set (CMAKE_BUILD_TYPE Release)
    endif ()

    if(ANDROID OR IOS)
        set(IS_TARGET_MOBILE 1)
        add_definitions(-DIS_TARGET_MOBILE)
    endif()

    if(EMSCRIPTEN OR NACL)
        set(IS_TARGET_WEB 1)
        add_definitions(-DIS_TARGET_WEB)
    endif()

    if(NACL OR ANDROID OR IOS OR EMSCRIPTEN)
        set(GLES2 1)
        add_definitions(-DGLES2)
    endif()

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    if(WIN32)
        if (MSVC)
            add_definitions("/Zi /wd4519")
            add_definitions( "/MP" )
            add_definitions (-D_CRT_SECURE_NO_WARNINGS)
            add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
            add_definitions( -DNOMINMAX)

            #We statically link to reduce dependancies
            FOREACH(flag_var    CMAKE_CXX_FLAGS 
                                CMAKE_CXX_FLAGS_DEBUG 
                                CMAKE_CXX_FLAGS_RELEASE 
                                CMAKE_CXX_FLAGS_MINSIZEREL 
                                CMAKE_CXX_FLAGS_RELWITHDEBINFO
                                CMAKE_C_FLAGS_DEBUG
                                CMAKE_C_FLAGS_RELEASE
                                CMAKE_C_FLAGS_MINSIZEREL 
                                CMAKE_C_FLAGS_RELWITHDEBINFO)

                IF(${flag_var} MATCHES "/MD")
                    STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
                ENDIF(${flag_var} MATCHES "/MD")
                IF(${flag_var} MATCHES "/MDd")
                    STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
                ENDIF(${flag_var} MATCHES "/MDd")

            ENDFOREACH(flag_var)
        ENDIF(MSVC)
    endif(WIN32)

    if (CMAKE_CXX_COMPILER MATCHES ".*clang[++]")
        set(CMAKE_COMPILER_IS_CLANGXX 1)
    endif ()

    if(CMAKE_COMPILER_IS_CLANGXX)
        message("detected clang compiler")
        if(APPLE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++ -std=c++11 -stdlib=libc++")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "libc++")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
        endif()
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        message("detected GNU compiler")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
    elseif(EMSCRIPTEN)
        add_definitions(-s ALLOW_MEMORY_GROWTH=${ALLOW_MEMORY_GROWTH})
        message("detected emscripten compiler")
        set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-warn-absolute-paths -Wno-logical-op-parentheses")
        set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-warn-absolute-paths -Wno-logical-op-parentheses")
        set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g${EMS_DEBUG_LEVEL} -Wno-warn-absolute-paths -Wno-logical-op-parentheses")
        set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g${EMS_DEBUG_LEVEL} -Wno-warn-absolute-paths -Wno-logical-op-parentheses")
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
        add_definitions(-DEMSCRIPTEN)
    endif()

    if(APPLE)
        if(IOS)
            add_definitions(-DIOS)
            set (CMAKE_XCODE_EFFECTIVE_PLATFORMS -iphoneos -iphonesimulator)
            set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_64_BIT)")
            set (CMAKE_OSX_SYSROOT iphoneos) #Set Base SDK to "Latest iOS"
            set (MACOSX_BUNDLE_GUI_IDENTIFIER com.xxxx.xxxx.\${PRODUCT_NAME:xxxxxx})
        elseif(XCODE)
            set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_64_BIT)")
            set (CMAKE_OSX_SYSROOT "macosx")  #Set Base SDK to "Latest OS X"
        endif()
    endif()

    if(NACL)

        set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_NDEBUG")
        set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_NDEBUG")
        set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
        set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")

        if(WIN32)
            #NaCl libraries on Windows have ".a" extension not ".lib"
            set(CMAKE_LINK_LIBRARY_SUFFIX "")
        endif() 

        set(CMAKE_EXECUTABLE_SUFFIX ".pexe")

    endif()

endmacro (setup_common)

macro (setup_common_ios_properties)
    if(IOS)
        # Maybe you want to change these
        set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "5.1")
        set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_VALID_ARCHS "armv7;armv7s")
        set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
        set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
endmacro (setup_common_ios_properties)

##################################################################################
##################################################################################
##################################################################################
macro (setup_executable)

    get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    PROJECT(${PROJECT_NAME})

    file(GLOB src "*.cpp")
    file(GLOB hdr "*.h")
    set(data_dir ${CMAKE_CURRENT_SOURCE_DIR}/data)

    if(NACL)
        
        add_executable(${PROJECT_NAME} ${src} ${hdr})
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})
        target_link_libraries(${PROJECT_NAME} ppapi_cpp ppapi ppapi_gles2 pthread nacl_io)

        set(nacl_host_dir ${CMAKE_SOURCE_DIR}/host/nacl)

        if(EXISTS "${nacl_host_dir}")
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${nacl_host_dir} ${CMAKE_CURRENT_BINARY_DIR}
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

            configure_file(${nacl_host_dir}/app_nmf.in ${CMAKE_CURRENT_BINARY_DIR}/app.nmf)
        endif()

        if(EXISTS "${data_dir}")
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/data
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
        endif()

        pnacl_finalise(${PROJECT_NAME})

    elseif(ANDROID)
        
        add_library(${PROJECT_NAME} SHARED ${src} ${hdr})
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})
        target_link_libraries(${PROJECT_NAME} dl GLESv2 log android EGL)

        if(USE_SDL)
            set(android_host_dir ${CMAKE_SOURCE_DIR}/host/androidSDL)
        else()
            set(android_host_dir ${CMAKE_SOURCE_DIR}/host/android)
        endif()

        if(EXISTS "${android_host_dir}")
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${android_host_dir} ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

            configure_file(${android_host_dir}/src/com/nsg/NSGActivity.in ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost/src/com/nsg/${PROJECT_NAME}/${PROJECT_NAME}Activity.java)
            configure_file(${android_host_dir}/AndroidManifest.in ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost/AndroidManifest.xml)
            configure_file(${android_host_dir}/build.in ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost/build.xml @ONLY)

            macro ( generate_local_properties_for_ant)
                set (LOCAL_PROPERTIES_FILE ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost/local.properties)
                file(WRITE ${LOCAL_PROPERTIES_FILE} "# Autogenerated by cmake\n")
                file(APPEND ${LOCAL_PROPERTIES_FILE} "# Location of the SDK. This is only used by Ant\n")
                file(APPEND ${LOCAL_PROPERTIES_FILE} "sdk.dir=$ENV{ANDROID_SDK}\n")
            endmacro( generate_local_properties_for_ant)

            generate_local_properties_for_ant()

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBRARY_OUTPUT_PATH_ROOT}/libs ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost/libs
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMENT "Copying libraries to AndroidHost" VERBATIM)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E remove ${LIBRARY_OUTPUT_PATH_ROOT}/libs/${ANDROID_NDK_ABI_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMENT "Removing generated library from libs (just to not have a copy of it in the next project)" VERBATIM)


            if(EXISTS "${data_dir}")
                add_custom_command(
                    TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost/assets/data
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
            endif()

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND $ENV{ANT_HOME}/bin/ant debug
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost
                    COMMENT "Generating APK (${PROJECT_NAME})" VERBATIM)

            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND $ENV{ANDROID_SDK}/platform-tools/adb -d install -r "bin/${PROJECT_NAME}-debug.apk"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost
                    COMMENT "Installing APK (${PROJECT_NAME}) on device" VERBATIM)

        endif()

    elseif(APPLE)

        if(EXISTS "${data_dir}")
            add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${src} ${hdr} ${data_dir})
            set_source_files_properties(${data_dir} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        else()
            add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${src} ${hdr})
        endif()

        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})

        setup_common_ios_properties()

        if(IOS)
            set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-framework AudioToolbox -framework CoreAudio -framework CoreGraphics -framework Foundation -framework OpenGLES -framework QuartzCore -framework UIKit")
        else()
            set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-framework AudioUnit -framework Carbon -framework Cocoa -framework CoreAudio -framework ForceFeedback -framework IOKit -framework OpenGL -framework CoreServices")
        endif()

    elseif(EMSCRIPTEN)

        set(CMAKE_EXECUTABLE_SUFFIX ".bc")
        add_executable(${PROJECT_NAME} ${src} ${hdr} )
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})

        if(EXISTS "${data_dir}")
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/data
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
        endif()

        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND $ENV{EMSCRIPTEN}/emcc ${PROJECT_NAME}.bc -o ${PROJECT_NAME}.html -g${EMS_DEBUG_LEVEL} -s ALLOW_MEMORY_GROWTH=${ALLOW_MEMORY_GROWTH} --embed-file data 
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating HTML with Emscripten" VERBATIM)

        # add_custom_command(
        #     TARGET ${PROJECT_NAME} POST_BUILD
        #         COMMAND $ENV{EMSCRIPTEN}/emcc ${PROJECT_NAME}.bc -o ${PROJECT_NAME}.html -g${EMS_DEBUG_LEVEL} -s ALLOW_MEMORY_GROWTH=${ALLOW_MEMORY_GROWTH} --preload-file data
        #         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        #         COMMENT "Generating HTML with Emscripten" VERBATIM)


    else()

        add_executable(${PROJECT_NAME} ${src} ${hdr} )
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})

        if(UNIX)
            target_link_libraries(${PROJECT_NAME} ${X11_Xxf86vm_LIB} ${X11_Xrandr_LIB} ${X11_Xinput_LIB} ${X11_Xcursor_LIB})
        endif()

        if(EXISTS "${data_dir}")
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/data
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
        endif()

    endif()


endmacro (setup_executable)

##################################################################################
##################################################################################
##################################################################################
macro (setup_tool)
   setup_executable()
endmacro (setup_tool)

##################################################################################
##################################################################################
##################################################################################
macro (setup_library)
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${src} ${hdr} ${assets})
    setup_common_ios_properties()
endmacro (setup_library)

##################################################################################
##################################################################################
##################################################################################
macro (setup_sample)
    setup_executable()
    target_link_libraries(${PROJECT_NAME} NSG)
    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "samples")
endmacro (setup_sample)

##################################################################################
##################################################################################
##################################################################################
macro (setup_test)
    setup_executable()
    target_link_libraries(${PROJECT_NAME} NSG)
    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")
    add_test(${PROJECT_NAME} ${PROJECT_NAME})
endmacro (setup_test)

##################################################################################
##################################################################################
##################################################################################
macro ( mark_as_disable_and_internal _var )
    set ( ${_var} OFF CACHE INTERNAL "disable and hide this!" FORCE )
endmacro( mark_as_disable_and_internal _var )

##################################################################################
##################################################################################
##################################################################################
macro ( mark_as_enable _var )
    set ( ${_var} ON CACHE BOOL "enable this!" FORCE )
endmacro( mark_as_enable _var )

##################################################################################
##################################################################################
##################################################################################
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

MACRO(ADDSOURCESLIST result curdir)
    INCLUDE_DIRECTORIES(${curdir})
    SET(src "")
    FILE(GLOB src "${curdir}/*.c" "${curdir}/*.h")
    SET(${result} ${${result}} ${src})
ENDMACRO()