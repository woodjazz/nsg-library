[![Build Status](https://travis-ci.org/woodjazz/nsg-library.svg?branch=master)](https://travis-ci.org/woodjazz/nsg-library)
[![Build status](https://ci.appveyor.com/api/projects/status/low8yga069l94b43?svg=true)](https://ci.appveyor.com/project/woodjazz/nsg-library)
<a href="https://scan.coverity.com/projects/woodjazz-nsg-library">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/6406/badge.svg"/>
</a>

---------------------------
FEATURES
---------------------------
>- Cross-platform. Currently runs on Windows, Linux, Mac OS X, Android, iOS, and HTML5 with Emscripten toolchain.
- OpenGL 2.0, OpenGL ES 2.0 or WebGL rendering.
- Scene graph.
- Keyframe animation.
- Skeletal animation (with hardware skinning).
- Point, spot and directional lights.
- Shadow mapping for all light types; cascaded shadow maps for directional lights.
- Post-processing: global and local by object.
- Events controlled by signals and slots.
- Physics (using bullet library).
- HTTP requests.
- Image loading.
- IMGUI.
- Audio.
- Exporter for blender files.

---------------------------
BUILDING: ALL PLATFORMS
---------------------------
Install CMake 2.8 or superior.

You need a c++11 compliant compiler.

--------------------------------
BUILDING: ANDROID (Ubuntu 14.04)
--------------------------------

sudo apt-get install openjdk-7-jdk

In order to install Android SDK:
sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0 lib32stdc++6

Install SDK,NDK
Set the environment variables: ANDROID_SDK, ANDROID_NDK

For the SDK you will need Android 4.4.2 (API 19)
In order to start developing applications, you must install the Platform-tools
and at least one version (API 19) of the Android platform, using the SDK Manager.
(Or run: $ANDROID_SDK/tools/android update sdk --no-ui)

Install ant (http://ant.apache.org/bindownload.cgi)
Set the environment variables: ANT_HOME

--------------------------------
BUILDING: ANDROID (Windows/OSX)
--------------------------------

Install SDK,NDK,Java(JDK 1.7)

For the SDK you will need Android 4.4.2 (API 19)

Install ant (http://ant.apache.org/bindownload.cgi)

Set the environment variables: ANDROID_SDK, ANDROID_NDK, ANT_HOME and JAVA_HOME 

IMPORTANT NOTE FOR WINDOWS' USERS: use UNIX syntax for paths when setting the environment variables. (Do not use \ use / instead)

-------------------------
BUILDING: Linux (Debian/Ubuntu)
-------------------------
apt-get install xorg-dev

apt-get install libglu1-mesa-dev

apt-get install libasound2-dev

-------------------------
BUILDING: Emscripten
-------------------------
Install SDK: http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html

Set environment variable: HOME_EMSCRIPTEN

For chrome you will need a HTTP server. For instance: python -m SimpleHTTPServer 8000

-------------------------
BUILDING: Windows
-------------------------
Install last Visual Studio

-------------------------
BUILDING: OSX/IOS
-------------------------
Install last XCode

-------------------------
Blender export
-------------------------
In order to convert .blend files to .xml you shall install blender (last version)
and define the following environment variable: BLENDER_BIN. 
BLENDER_BIN has to point to the directory where the blender executable is placed.
(See ExportBlendTool.cmake to see how cmake is looking for the blender executable)

-------------------------
Samples
-------------------------
These are some examples (from the samples folder) generated with the Emscripten toolchain.

- [animation](http://woodjazz.github.io/samples/animation/animation.html)
- [loader](http://woodjazz.github.io/samples/loader/loader.html)
- [momo](http://woodjazz.github.io/samples/momo/momo.html)
- [momoanimations](http://woodjazz.github.io/samples/momoanimations/momoanimations.html)
- [momohome](http://woodjazz.github.io/samples/momohome/momohome.html)
- [planet](http://woodjazz.github.io/samples/planet/planet.html)

>For loader, momo and momoanimations you can move and rotate the camera around:

>- Press the [F key] over object. This will center the view on the selected point.
- Press the [C key] over object. This will center the view on the object's center.
- Hold [Alt key] + [Left Mouse Button] to orbit the camera around the current mouse position.
- Hold [Alt key] + [Shift key] + [Left Mouse Button] to zoom the view. This is the same as scrolling with your mouse wheel.
- Hold [Shift key] + [Left Mouse Button] to drag camera around.
- Press [R key] to reset the camera position.

>For momohome and planet you can connect a XBox360 gamepad to one USB and play around... (You can test your gamepad [here](http://html5gamepad.com/))