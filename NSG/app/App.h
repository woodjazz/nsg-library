/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#pragma once

#include "Types.h"
#include "VertexData.h"
#include "Tick.h"
#include "Context.h"
#include "Singleton.h"
#include "IMGUI.h"
#include "MapAndVector.h"
#include "SignalSlots.h"
#include <map>
#include <functional>

class AAssetManager;
class ANativeActivity;
namespace pp
{
    class Var;
}

namespace NSG
{
    struct InternalApp;
    class App : public IMGUI::IWindow, public Singleton<App>
    {
    public:
        App();
        App(PAppConfiguration configuration);
        virtual ~App();
        void SetCommandLineParameters(int argc, char* argv[]);
        virtual bool ShallExit() const;
        virtual void DropFile(const std::string& filePath);
        void DoTick(float delta);
        std::pair<int, int> GetViewSize() const;
        float GetDeltaTime() const;
        virtual void HandleMessage(const pp::Var& var_message);
        void SetAssetManager(AAssetManager* pAAssetManager);
        AAssetManager* GetAssetManager();
        PScene GetOrCreateScene(const std::string& name);
        PBoxMesh CreateBoxMesh(float width = 2, float height = 2, float depth = 2, int resX = 2, int resY = 2, int resZ = 2);
        PCircleMesh CreateCircleMesh(float radius = 1, int res = 8);
        PEllipseMesh CreateEllipseMesh(float width = 2, float height = 1, int res = 8);
        PModelMesh GetOrCreateModelMesh(const std::string& name);
        PPlaneMesh CreatePlaneMesh(float width = 2, float height = 2, int columns = 2, int rows = 2);
        PRectangleMesh CreateRectangleMesh(float width = 2, float height = 1);
        PRoundedRectangleMesh CreateRoundedRectangleMesh(float radius = 1, float width = 2, float height = 2, int res = 8);
        PSphereMesh CreateSphereMesh(float radius = 1, int res = 8);
        PTextMesh CreateTextMesh(const std::string& textureFilename, bool dynamic = true);
        PMaterial CreateMaterial(const std::string& name);
        PMaterial GetOrCreateMaterial(const std::string& name);
        PResourceFile GetOrCreateResourceFile(const Path& path);
        PTexture GetOrCreateTextureFile(const Path& path, TextureFlags flags = (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
        PProgram GetOrCreateProgram(const std::string& name);
        const std::vector<PMesh>& GetMeshes() const;
        PMesh GetMesh(const std::string& name) const;
        const std::vector<PMaterial>& GetMaterials() const;
        int GetMaterialSerializableIndex(const PMaterial& material) const;
        int GetMeshSerializableIndex(const PMesh& mesh) const;
        void ClearAll();
        void ViewChanged(int width, int height);
        void AppEnterBackground();
        void AppEnterForeground();
		PScene GetCurrentScene() const { return currentScene_; }
		void SetCurrentScene(PScene scene) { currentScene_ = scene; }
    public:
		SignalViewChanged::PSignal signalViewChanged_;
		SignalMouseMoved::PSignal signalMouseMoved_;
		SignalMouseDown::PSignal signalMouseDown_;
		SignalMouseUp::PSignal signalMouseUp_;
		SignalMouseWheel::PSignal signalMouseWheel_;
		SignalKey::PSignal signalKey_;
		SignalChar::PSignal signalChar_;
		SignalMultiGesture::PSignal signalMultiGesture_;
		SignalStart::PSignal signalStart_;
		SignalUpdate::PSignal signalUpdate_;
		
    private:
		void Start(int argc, char* argv[]);
		void Update();
		virtual void RenderFrame();
        AAssetManager* pAAssetManager_;
        int32_t width_;
        int32_t height_;
        float deltaTime_; // Fixed time in seconds (1/AppConfiguration::fps_)
        PContext context_;
        PAppConfiguration configuration_;
		PScene currentScene_;
        int argc_;
        char** argv_;
        MapAndVector<std::string, Scene> scenes_;
        MapAndVector<std::string, Mesh> meshes_;
        MapAndVector<std::string, Material> materials_;
        MapAndVector<std::string, Program> programs_;
        friend struct InternalApp;
    };

    struct InternalApp : public Tick
    {
        PApp pApp_;
        InternalApp(App* pApp);
        ~InternalApp();
        virtual void InitializeTicks() override;
        virtual void BeginTicks() override;
        virtual void DoTick(float delta) override;
        virtual void EndTicks() override;
        void ViewChanged(int32_t width, int32_t height);
        void OnMouseMove(float x, float y);
        void OnMouseWheel(float x, float y);
        void OnMouseDown(int button, float x, float y);
        void OnMouseUp(int button, float x, float y);
        void OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers);
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void RenderFrame();
        bool ShallExit() const;
        void AppEnterBackground();
        void AppEnterForeground();
        void InvalidateContext();
        void HandleMessage(const pp::Var& var_message);
        void SetAssetManager(AAssetManager* pAAssetManager);
        void SetActivity(ANativeActivity* pActivity);
        void DropFile(const std::string& filePath);
    };
}

