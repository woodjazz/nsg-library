# /*
# -------------------------------------------------------------------------------
# This file is part of nsg-library.
# http://github.com/woodjazz/nsg-library

# Copyright (c) 2014-2015 Néstor Silveira Gorski

# -------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.

# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:

# 1. The origin of this software must not be misrepresented; you must not
# claim that you wrote the original software. If you use this software
# in a product, an acknowledgment in the product documentation would be
# appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
# misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
# -------------------------------------------------------------------------------
# */

include(${CMAKE_SOURCE_DIR}/cmake/modules/Common.cmake)

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(BLENDER_EXECUTABLE "/Applications/Blender275/blender.app/Contents/MacOS/blender")
elseif(WIN32)
  set(BLENDER_EXECUTABLE "D:/tmp/Blender-2.75-windows64/blender.exe")
elseif(UNIX AND NOT APPLE) 
  message(WARNING, "Unknown blender executable")
else()
  message(WARNING, "Unknown OS. Cannot set blender executable")
endif() 

set(EXPORT_SCRIPT ${ROOT_SOURCE_DIR}/tools/blenderexport/Export.py)


FUNCTION(BUILD_DATA_TOOL dir)
  SUBDIRLIST(SUBDIRS ${CMAKE_SOURCE_DIR}/${dir})
  FOREACH(subdir ${SUBDIRS})
    get_filename_component(INPUTDIR "${dir}/${subdir}" ABSOLUTE)
    set(ARTDATA "${INPUTDIR}/art")
    if(EXISTS ${ARTDATA})
      set(FILEDATA "${INPUTDIR}/data")
      file(GLOB BLEND_FILES "${ARTDATA}/*.blend")
      FOREACH(blendfile ${BLEND_FILES})
        execute_process(COMMAND ${BLENDER_EXECUTABLE} ${blendfile} --background --python ${EXPORT_SCRIPT})
      ENDFOREACH()
    endif()
  ENDFOREACH()
ENDFUNCTION(BUILD_DATA_TOOL)

BUILD_DATA_TOOL(samples)
BUILD_DATA_TOOL(tests)