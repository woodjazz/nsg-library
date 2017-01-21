#-------------------------------------------------------------------------------
#This file is part of nsg-library.
#http://github.com/woodjazz/nsg-library

#Copyright (c) 2014-2016 Néstor Silveira Gorski

#-------------------------------------------------------------------------------
#This software is provided 'as-is', without any express or implied
#warranty. In no event will the authors be held liable for any damages
#arising from the use of this software.

#Permission is granted to anyone to use this software for any purpose,
#including commercial applications, and to alter it and redistribute it
#freely, subject to the following restrictions:

#1. The origin of this software must not be misrepresented; you must not
#claim that you wrote the original software. If you use this software
#in a product, an acknowledgment in the product documentation would be
#appreciated but is not required.
#2. Altered source versions must be plainly marked as such, and must not be
#misrepresented as being the original software.
#3. This notice may not be removed or altered from any source distribution.
#-------------------------------------------------------------------------------
macro (setup_common)
    message(STATUS "SYSTEM_NAME = ${CMAKE_SYSTEM_NAME} SYSTEM_VERSION = ${CMAKE_SYSTEM_VERSION}")
    if (CMAKE_GENERATOR STREQUAL Xcode)
       set (XCODE TRUE)
    endif ()
    if(ANDROID OR EMSCRIPTEN)
        if (NOT CMAKE_BUILD_TYPE)
            message("Unknown build type. Building Release version by default")
            set (CMAKE_BUILD_TYPE Release)
        endif ()
    endif()
    message(STATUS "Building ${CMAKE_BUILD_TYPE} version")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
    #set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})    
    if(ANDROID)
        message(STATUS "detected ANDROID")
        set(IS_TARGET_MOBILE 1)
        add_definitions(-DIS_TARGET_ANDROID)
        add_definitions(-DIS_TARGET_MOBILE)
    elseif(IOS) 
        message(STATUS "detected IOS")
        add_definitions(-DIOS)
        add_definitions(-DIS_TARGET_IOS)
        add_definitions(-DIS_TARGET_APPLE)
        set(IS_TARGET_MOBILE 1)
        add_definitions(-DIS_TARGET_MOBILE)
    elseif(EMSCRIPTEN)
        message(STATUS "detected EMSCRIPTEN")
        set(IS_TARGET_WEB 1)
        add_definitions(-DIS_TARGET_WEB)
        add_definitions(-DEMSCRIPTEN)
        add_definitions(-DIS_TARGET_EMSCRIPTEN)
    elseif(WIN32)
        message(STATUS "detected WINDOWS")
        set(IS_TARGET_WINDOWS 1)
        add_definitions(-DIS_TARGET_WINDOWS)
    elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
        message(STATUS "detected OSX")
        set(IS_TARGET_OSX 1)
        add_definitions(-DIS_TARGET_OSX)
        add_definitions(-DIS_TARGET_APPLE)
    elseif(UNIX AND NOT APPLE)
        message(STATUS "detected LINUX")
        set(IS_TARGET_LINUX 1)
        add_definitions(-DIS_TARGET_LINUX)
    endif()
    if(IS_TARGET_MOBILE OR IS_TARGET_WEB)
        set(GLES2 1)
        add_definitions(-DGLES2)
    endif()
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    if(WIN32)  
        if (MSVC)
            add_definitions("/Zi /wd4519")
            add_definitions("/EHa")
            add_definitions("/MP")
            add_definitions(-D_CRT_SECURE_NO_WARNINGS)
            add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
            add_definitions(-DNOMINMAX)

            #We statically link to reduce dependancies
            foreach(flag_var    CMAKE_CXX_FLAGS 
                                CMAKE_CXX_FLAGS_DEBUG 
                                CMAKE_CXX_FLAGS_RELEASE 
                                CMAKE_CXX_FLAGS_MINSIZEREL 
                                CMAKE_CXX_FLAGS_RELWITHDEBINFO
                                CMAKE_C_FLAGS_DEBUG
                                CMAKE_C_FLAGS_RELEASE
                                CMAKE_C_FLAGS_MINSIZEREL 
                                CMAKE_C_FLAGS_RELWITHDEBINFO)

                if(${flag_var} MATCHES "/MD")
                    string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
                endif(${flag_var} MATCHES "/MD")
                if(${flag_var} MATCHES "/MDd")
                    string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
                endif(${flag_var} MATCHES "/MDd")

            endforeach(flag_var)
        endif(MSVC)
    endif(WIN32)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_COMPILER_IS_CLANGXX 1)
    endif ()
    if (CMAKE_CXX_COMPILER MATCHES ".*clang[++]")
        set(CMAKE_COMPILER_IS_CLANGXX 1)
    endif ()
    if(CMAKE_COMPILER_IS_CLANGXX)
        message(STATUS "detected clang compiler")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
        if(APPLE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register -x objective-c++ -std=c++11 -stdlib=libc++")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -x objective-c")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "libc++")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-deprecated-register -D_DEBUG")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libstdc++ -pthread")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
        endif()
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        message(STATUS "detected GNU compiler")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register -std=gnu++11 -pthread")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-deprecated-register -D_DEBUG")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
    elseif(EMSCRIPTEN)
        message(STATUS "detected EMSCRIPTEN compiler")
        set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
        set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
        set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-deprecated-register -Wno-logical-op-parentheses")
        set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-logical-op-parentheses")
        set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g${EMS_DEBUG_LEVEL} -Wno-deprecated-register -Wno-logical-op-parentheses -D_DEBUG")
        set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g${EMS_DEBUG_LEVEL} -Wno-logical-op-parentheses -D_DEBUG")
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind -std=c++11 -stdlib=libc++")
    endif()
    message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
    if(${CMAKE_BUILD_TYPE} MATCHES "Release")
        message(STATUS "CMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}")
        message(STATUS "CMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}")
    else()
        message(STATUS "CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
        message(STATUS "CMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}")
        message(STATUS "CMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}")
        message(STATUS "CMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}")
    endif()
    if(APPLE)
        if(IOS)
            set (CMAKE_XCODE_EFFECTIVE_PLATFORMS -iphoneos -iphonesimulator)
            set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_64_BIT)")
            set (CMAKE_OSX_SYSROOT iphoneos) #Set Base SDK to "Latest iOS"
            set (MACOSX_BUNDLE_GUI_IDENTIFIER com.xxxx.xxxx.\${PRODUCT_NAME:xxxxxx})
        elseif(XCODE)
            set (CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_64_BIT)")
            set (CMAKE_OSX_SYSROOT "macosx")  #Set Base SDK to "Latest OS X"
        endif()
    endif()
endmacro ()

macro(setup_common_ios_properties)
    if(APPLE)
        if(IOS)
            # Maybe you want to change these
            set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "6.0")
            set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_VALID_ARCHS "armv7;armv7s")
            set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
            set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
        else()
            set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "10.8")
        endif()
    endif(APPLE)
endmacro()

macro(setup_executable isQTProject)
    get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    project(${PROJECT_NAME})
    file(GLOB src "*.cpp")
    file(GLOB hdr "*.h")
    set(data_dir ${CMAKE_CURRENT_SOURCE_DIR}/data)
    set(art_dir ${CMAKE_CURRENT_SOURCE_DIR}/art)
    if(${isQTProject})
        file(GLOB qrc_files "*.qrc")
        qt5_add_resources(qt_resources ${qrc_files})
        add_definitions (-DHAS_QT5)
        # The Qt5Widgets_INCLUDES also includes the include directories for
        # dependencies QtCore and QtGui
        include_directories(${Qt5Widgets_INCLUDE_DIRS})
        include_directories(${Qt5Quick_INCLUDES_DIRS})
        # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
        add_definitions(${Qt5Widgets_DEFINITIONS})
        add_definitions(${Qt5Quick_DEFINITIONS})
        # Executables fail to build with Qt 5 in the default configuration
        # without -fPIE. We add that here.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Quick_EXECUTABLE_COMPILE_FLAGS}")
        #message("${Qt5Widgets_INCLUDE_DIRS} ${Qt5Quick_INCLUDES_DIRS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Quick_EXECUTABLE_COMPILE_FLAGS}")
        list(APPEND LIBRARIES_2_LINK ${Qt5Widgets_LIBRARIES} ${Qt5Quick_LIBRARIES})
        #message("${Qt5Widgets_LIBRARIES} ${Qt5Quick_LIBRARIES} ${qt_resources} ${src} ${Qt5Widgets_INCLUDES} ${Qt5Quick_INCLUDES}")
    endif()
    if(ANDROID)
        set(android_host_dir ${ROOT_SOURCE_DIR}/cmake/host/android)
        add_library(${PROJECT_NAME} SHARED ${src} ${hdr})
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})
        target_link_libraries(${PROJECT_NAME} dl GLESv2 log android EGL)
        # Use: $ANDROID_NDK/ndk-depends lib${PROJECT_NAME}.so to see dependencies
        #target_link_libraries(${PROJECT_NAME} log m dl c android EGL GLESv2)
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
        if(IOS)
            set(IS_EXECUTABLE_A_BUNDLE 1)
        endif()
        if(IS_EXECUTABLE_A_BUNDLE)
            set (EXECUTABLE_TYPE MACOSX_BUNDLE)
        endif()
        if(EXISTS "${data_dir}")
            if(IS_EXECUTABLE_A_BUNDLE)
                add_executable(${PROJECT_NAME} ${EXECUTABLE_TYPE} ${src} ${hdr} ${data_dir} ${qt_resources})
                set_source_files_properties(${data_dir} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
            else()
                add_executable(${PROJECT_NAME} ${EXECUTABLE_TYPE} ${src} ${hdr} ${qt_resources})
            endif()
        else()
            add_executable(${PROJECT_NAME} ${EXECUTABLE_TYPE} ${src} ${hdr} ${qt_resources})
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
        add_executable(${PROJECT_NAME} ${src} ${hdr})
        set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS "1")
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND $ENV{EMSCRIPTEN}/emcc ${PROJECT_NAME}.bc  --bind -s DEMANGLE_SUPPORT=1 -s ALLOW_MEMORY_GROWTH=1 -o ${PROJECT_NAME}.html --embed-file data 
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating HTML with Emscripten" VERBATIM)
    else()
        add_executable(${PROJECT_NAME} ${src} ${hdr} ${qt_resources} )
        if(IS_TARGET_LINUX AND NEEDS_X11)
            find_package(X11 REQUIRED)
            set(LIBRARIES_2_LINK ${LIBRARIES_2_LINK} ${CMAKE_DL_LIBS} ${X11_X11_LIB} ${X11_Xxf86vm_LIB} ${X11_Xrandr_LIB} ${X11_Xinput_LIB} ${X11_Xcursor_LIB} )
        endif()
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_2_LINK})
    endif()
    if(EXISTS "${data_dir}" OR EXISTS "${art_dir}")
        set(DATA_TARGET ${PROJECT_NAME}dataDir)
        if(ANDROID AND EXISTS "${android_host_dir}")
            add_custom_target(${DATA_TARGET} 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/AndroidHost/assets/data
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMENT "Copying data directory" VERBATIM)
        elseif(APPLE)# AND IS_EXECUTABLE_A_BUNDLE)
            add_custom_target(${DATA_TARGET} 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/data
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMENT "Copying data directory" VERBATIM)
        else()                                    
            add_custom_target(${DATA_TARGET} 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${data_dir} ${CMAKE_CURRENT_BINARY_DIR}/data
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMENT "Copying data directory" VERBATIM)
        endif()
        set_property(TARGET ${DATA_TARGET} PROPERTY FOLDER "DATA_COPIES")
        add_dependencies(${PROJECT_NAME} ${DATA_TARGET})
    endif()
endmacro (setup_executable)

macro (setup_tool)
    setup_executable(FALSE)
    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tools")
endmacro ()

macro (setup_app_tool)
    set(IS_EXECUTABLE_A_BUNDLE 1)   
    setup_executable(FALSE)
    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tools")
endmacro ()

macro (setup_library)
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${src} ${hdr} ${assets} ${shaders})
    if(IS_TARGET_LINUX AND NEEDS_X11)
        find_package(X11 REQUIRED)
        list(APPEND LIBRARIES_2_LINK ${CMAKE_DL_LIBS}) #needed to fix cycle dependencies
    endif()
    setup_common_ios_properties()
endmacro ()

macro (setup_sample)
    set(IS_EXECUTABLE_A_BUNDLE 1)
    setup_executable(FALSE)
    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "samples")
endmacro ()

macro (setup_qt_sample)
    if (NOT CMAKE_GENERATOR STREQUAL Xcode)
	    if (NOT CMAKE_CROSSCOMPILING OR DEFINED ENV{OE_QMAKE_PATH_HOST_BINS})
            find_package(Qt5Widgets )
            if(Qt5Widgets_FOUND)
                find_package(Qt5Quick REQUIRED)
                set(IS_EXECUTABLE_A_BUNDLE 1)
                aux_source_directory(. SRC_LIST)
                # Tell CMake to run moc when necessary:
                set(CMAKE_AUTOMOC ON)
                # As moc files are generated in the binary dir, tell CMake
                # to always look for includes there:
                set(CMAKE_INCLUDE_CURRENT_DIR ON)
                setup_executable(TRUE)
                set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "samples")
            endif()
        endif()
    endif()
endmacro ()

macro (setup_test)
    #set(IS_EXECUTABLE_A_BUNDLE 1)
    setup_executable(FALSE)
    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")
    add_test(${PROJECT_NAME} ${PROJECT_NAME})
endmacro ()

macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        set(dirlist ${dirlist} ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()
