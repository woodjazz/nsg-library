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

For EGL you need: sudo apt-get install libgles2-mesa-dev

You can add installed compiler as selectable alternatives with:

sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang-3.5 100

sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.5 100

If you have g++ and clang++ installed you can switch between them with:

sudo update-alternatives --config c++

For clang you probably will need:

apt-get install libc++-dev

Note: if you are running Linux under VMPlayer 12 then turn off OpenGL 3.3 setting the environment variable SVGA_VGPU10=0 (It will work as OpenGL 2.1.)

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
These are some examples (from the samples folder):

- animation

```cpp
#include "NSG.h"

int NSG_MAIN(int argc, char* argv[]) {
    using namespace NSG;
    auto engine = Engine::Create();
    auto& conf = engine->GetAppConfiguration();
    conf.platform_ = AppConfiguration::Platform::UseEGL;

    PAnimationController acontrol;
    auto window = Window::Create();

    LoaderXML loader("loader");
    auto slotLoaded =
        loader.Load(Resource::GetOrCreateClass<ResourceFile>("data/duck.xml"))
            ->Connect([&]() {
                auto scene = loader.GetScene(0);
                auto objNode = scene->GetOrCreateChild<SceneNode>("LOD3sp");
                auto objPos = objNode->GetGlobalPosition();
                auto objBB = objNode->GetWorldBoundingBox();
                objBB.max_ *= 1.75f;
                objBB.min_ *= 1.75f;
                auto camera = scene->GetOrCreateChild<Camera>("camera1");
                // camera->SetGlobalPosition(Vector3(0, objBB.max_.y,
                // objBB.max_.z));
                // camera->SetGlobalLookAtPosition(objPos);
                auto control = std::make_shared<CameraControl>(camera);

                auto animation = Animation::Create("anim0");
                AnimationTrack track;
                track.node_ = camera;
                track.channelMask_ = (int)AnimationChannel::POSITION |
                                     (int)AnimationChannel::ROTATION;

                {
                    AnimationKeyFrame key(0, camera.get());
                    track.keyFrames_.push_back(key);
                }

                {
                    auto node = std::make_shared<Node>("node0");
                    node->SetParent(camera->GetParent());
                    node->SetGlobalPosition(
                        Vector3(objBB.max_.x, objBB.max_.y, 0));
                    node->SetGlobalLookAtPosition(objPos);
                    AnimationKeyFrame key(2, node.get());
                    track.keyFrames_.push_back(key);
                }

                {
                    auto node = std::make_shared<Node>("node1");
                    node->SetParent(camera->GetParent());
                    node->SetGlobalPosition(
                        Vector3(0, objBB.max_.y, objBB.min_.z));
                    node->SetGlobalLookAtPosition(objPos);
                    AnimationKeyFrame key(4, node.get());
                    track.keyFrames_.push_back(key);
                }

                {
                    auto node = std::make_shared<Node>("node2");
                    node->SetParent(camera->GetParent());
                    node->SetGlobalPosition(
                        Vector3(objBB.min_.x, objBB.max_.y, 0));
                    node->SetGlobalLookAtPosition(objPos);
                    AnimationKeyFrame key(6, node.get());
                    track.keyFrames_.push_back(key);
                }

                animation->AddTrack(track);
                animation->SetLength(8);

                acontrol = std::make_shared<AnimationController>(camera);
                acontrol->Play(animation->GetName(), false);
                window->SetScene(scene);
            });
    return engine->Run();
}
```

![](http://woodjazz.github.io/images/animation.gif)


