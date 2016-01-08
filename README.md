[![Build Status](https://travis-ci.org/woodjazz/nsg-library.svg?branch=master)](https://travis-ci.org/woodjazz/nsg-library)
[![Build status](https://ci.appveyor.com/api/projects/status/low8yga069l94b43?svg=true)](https://ci.appveyor.com/project/woodjazz/nsg-library)
<a href="https://scan.coverity.com/projects/woodjazz-nsg-library">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/6406/badge.svg"/>
</a>

---------------------------
BUILDING: ALL PLATFORMS
---------------------------
Install CMake 2.8 or superior
You need a c++11 compliant compiler

---------------------------
BUILDING: ANDROID
---------------------------

Install SDK,NDK,Java(JDK 1.7)

For the SDK you will need Android 4.4.2 (API 19)

Install ant (http://ant.apache.org/bindownload.cgi)

Linux also needed: sudo apt-get install openjdk-7-jdk

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
Samples
-------------------------
These are some examples (from the samples folder) generated with the Emscripten toolchain.
They can run them in your browser...

- [animation](http://woodjazz.github.io/samples/animation/animation.html)
- [loader](http://woodjazz.github.io/samples/loader/loader.html)
- [momo](http://woodjazz.github.io/samples/momo/momo.html)
- [momoanimations](http://woodjazz.github.io/samples/momoanimations/momoanimations.html)
- [momohome](http://woodjazz.github.io/samples/momohome/momohome.html)
- [planet](http://woodjazz.github.io/samples/planet/planet.html)

For more info see me: [Néstor Silveira in GitHub](http://woodjazz.github.io/)