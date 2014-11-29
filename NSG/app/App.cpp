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

#include "App.h"
#include "AppConfiguration.h"
#include "AppStatistics.h"
#include "Log.h"
#include "IMGUI.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "TextMesh.h"
#include "Check.h"
#include "Context.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "FrameColorSelection.h"
#include "AppStatistics.h"
#include "IMGUIContext.h"
#include "Graphics.h"
#include "Scene.h"
#include "BoxMesh.h"
#include "CircleMesh.h"
#include "EllipseMesh.h"
#include "ModelMesh.h"
#include "PlaneMesh.h"
#include "RectangleMesh.h"
#include "RoundedRectangleMesh.h"
#include "SphereMesh.h"
#include "TextMesh.h"
#include "Material.h"
#include "Music.h"
#include "Program.h"
#include "RigidBody.h"
#include "ResourceFileManager.h"
#include "TextureFileManager.h"
#if NACL
#include "ppapi/cpp/var.h"
#endif
#include <sstream>

namespace NSG
{
    template <> App* Singleton<App>::this_ = nullptr;

    App::App()
        : width_(0),
          height_(0),
          argc_(0),
          argv_(nullptr),
          signalViewChanged_(new Signal<int, int>()),
          signalMouseMoved_(new Signal<float, float>()),
          signalMouseDown_(new Signal<int, float, float>()),
          signalMouseUp_(new Signal<int, float, float>()),
          signalMouseWheel_(new Signal<float, float>()),
          signalKey_(new Signal<int, int, int>()),
          signalChar_(new Signal<unsigned int>()),
          signalMultiGesture_(new Signal<int, float, float, float, float, int>()),
          signalStart_(new Signal<int, char**>()),
          signalUpdate_(new Signal<float>())
    {
        context_ = PContext(new Context);
        configuration_ = PAppConfiguration(new AppConfiguration);
    }

    App::App(PAppConfiguration configuration)
        : width_(0),
          height_(0)
    {
        context_ = PContext(new Context);
        configuration_ = configuration;
    }

    App::~App()
    {
        App::this_ = nullptr;
        TRACE_LOG("App Terminated");
    }

    void App::ViewChanged(int width, int height)
    {
        if (width_ != width || height_ != height)
        {
            width_ = width;
            height_ = height;

            signalViewChanged_->Run(width, height);
        }
    }

    bool App::ShallExit() const
    {
        return false;
    }

    void App::AppEnterBackground()
    {
        if (Music::this_ && configuration_->pauseMusicOnBackground_)
            Music::this_->Pause();
    }

    void App::AppEnterForeground()
    {
        if (Music::this_ && configuration_->pauseMusicOnBackground_)
            Music::this_->Resume();
    }

    void App::Start(int argc, char* argv[])
    {
		signalStart_->Run(argc, argv);
    }

    void App::RenderFrame()
    {
		if (currentScene_)
			currentScene_->Render();
    }

    void App::Update()
    {
		signalUpdate_->Run(deltaTime_);
		if (currentScene_)
			currentScene_->Update(deltaTime_);
    }

    void App::DropFile(const std::string& filePath)
    {
        TRACE_LOG("Dropped file:" << filePath);
    }

    void App::SetCommandLineParameters(int argc, char* argv[])
    {
        argc_ = argc;
        argv_ = argv;
    }

    std::pair<int, int> App::GetViewSize() const
    {
        return std::pair<int, int>(width_, height_);
    }

    float App::GetDeltaTime() const
    {
        return deltaTime_;
    }

    void App::SetAssetManager(AAssetManager* pAAssetManager)
    {
        pAAssetManager_ = pAAssetManager;
    }

    AAssetManager* App::GetAssetManager()
    {
        return pAAssetManager_;
    }

    void App::HandleMessage(const pp::Var& var_message)
    {
        #if NACL
        TRACE_LOG("App::HandleMessage");

        if (var_message.is_string())
        {
            std::string message = var_message.AsString();
        }
        #endif
    }
    void App::DoTick(float delta)
    {
        deltaTime_ = delta;
        IMGUI::DoTick();
        Update();
    }

    PScene App::GetOrCreateScene(const std::string& name)
    {
        PScene scene = scenes_.GetOrCreate(name);
        if(!currentScene_)
            currentScene_ = scene;
        return scene;
    }

    PBoxMesh App::CreateBoxMesh(float width, float height, float depth, int resX, int resY, int resZ)
    {
        std::stringstream ss;
        ss << "BoxMesh" << width << height << depth << resX << resY << resZ;
        PBoxMesh mesh = meshes_.GetOrCreateClass<BoxMesh>(ss.str());
        mesh->Set(width, height, depth, resX, resY, resZ);
        return mesh;
    }

    PCircleMesh App::CreateCircleMesh(float radius, int res)
    {
        std::stringstream ss;
        ss << "CircleMesh" << radius << res;
        PCircleMesh mesh = meshes_.GetOrCreateClass<CircleMesh>(ss.str());
        mesh->Set(radius, res);
        return mesh;
    }

    PEllipseMesh App::CreateEllipseMesh(float width, float height, int res)
    {
        std::stringstream ss;
        ss << "EllipseMesh" << width << height << res;
        PEllipseMesh mesh = meshes_.GetOrCreateClass<EllipseMesh>(ss.str());
        mesh->Set(width, height, res);
        return mesh;
    }

    PModelMesh App::GetOrCreateModelMesh(const std::string& name)
    {
        return meshes_.GetOrCreateClass<ModelMesh>(name);
    }

    PPlaneMesh App::CreatePlaneMesh(float width, float height, int columns, int rows)
    {
        std::stringstream ss;
        ss << "PlaneMesh" << width << height << columns << rows;
        PPlaneMesh mesh = meshes_.GetOrCreateClass<PlaneMesh>(ss.str());
        mesh->Set(width, height, columns, rows);
        return mesh;
    }

    PRectangleMesh App::CreateRectangleMesh(float width, float height)
    {
        std::stringstream ss;
        ss << "RectangleMesh" << width << height;
        PRectangleMesh mesh = meshes_.GetOrCreateClass<RectangleMesh>(ss.str());
        mesh->Set(width, height);
        return mesh;
    }

    PRoundedRectangleMesh App::CreateRoundedRectangleMesh(float radius, float width, float height, int res)
    {
        std::stringstream ss;
        ss << "RoundedRectangleMesh" << radius << width << height << res;
        PRoundedRectangleMesh mesh = meshes_.GetOrCreateClass<RoundedRectangleMesh>(ss.str());
        mesh->Set(radius, width, height, res);
        return mesh;
    }

    PSphereMesh App::CreateSphereMesh(float radius, int res)
    {
        std::stringstream ss;
        ss << "SphereMesh" << radius << res;
        PSphereMesh mesh = meshes_.GetOrCreateClass<SphereMesh>(ss.str());
        mesh->Set(radius, res);
        return mesh;
    }

    PTextMesh App::CreateTextMesh(const std::string& textureFilename, bool dynamic)
    {
        PTextMesh mesh = meshes_.CreateClass<TextMesh>(textureFilename);
        mesh->SetDynamic(dynamic);
        return mesh;
    }

    PMaterial App::CreateMaterial(const std::string& name)
    {
        return materials_.Create(name);
    }

    PMaterial App::GetOrCreateMaterial(const std::string& name)
    {
        return materials_.GetOrCreate(name);
    }

    PResourceFile App::GetOrCreateResourceFile(const Path& path)
    {
        return ResourceFileManager::this_->GetOrCreate(path);
    }

    PTexture App::GetOrCreateTextureFile(const Path& path, TextureFlags flags)
    {
        return TextureFileManager::this_->GetOrCreate(path);
    }

    PProgram App::GetOrCreateProgram(const std::string& name)
    {
        return programs_.GetOrCreate(name);
    }

    const std::vector<PMesh>& App::GetMeshes() const
    {
        return meshes_.GetConstObjs();
    }

    PMesh App::GetMesh(const std::string& name) const
    {
        return meshes_.Get(name);
    }

    const std::vector<PMaterial>& App::GetMaterials() const
    {
        return materials_.GetConstObjs();
    }

    int App::GetMaterialSerializableIndex(const PMaterial& material) const
    {
        int idx = -1;
        const std::vector<PMaterial>& materials = materials_.GetConstObjs();
        for (auto obj : materials)
        {
            if (obj->IsSerializable())
            {
                ++idx;
                if (obj == material)
                    break;
            }
        }

        return idx;
    }

    int App::GetMeshSerializableIndex(const PMesh& mesh) const
    {
        int idx = -1;
        for (auto obj : meshes_.GetConstObjs())
        {
            if (obj->IsSerializable())
            {
                ++idx;
                if (obj == mesh)
                    break;
            }
        }

        return idx;
    }

    void App::ClearAll()
    {
        currentScene_ = nullptr;
        scenes_.Clear();
        meshes_.Clear();
        materials_.Clear();
        programs_.Clear();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    InternalApp::InternalApp(App* pApp)
        : Tick(pApp->configuration_->fps_),
          pApp_(pApp)
    {
    }

    InternalApp::~InternalApp()
    {
        pApp_ = nullptr;
    }

    void InternalApp::InitializeTicks()
    {
        Context::this_->Initialize();

        TRACE_PRINTF("--- Begin Start ---\n");

        pApp_->Start(pApp_->argc_, pApp_->argv_);

        TRACE_PRINTF("--- End Start ---\n");
    }

    void InternalApp::BeginTicks()
    {
        Graphics::this_->BeginFrame();
    }

    void InternalApp::DoTick(float delta)
    {
        pApp_->DoTick(delta);
    }

    void InternalApp::EndTicks()
    {
        Graphics::this_->ClearAllBuffers();
        pApp_->RenderFrame();
        UniformsUpdate::ClearAllUpdates();
        IMGUI::Context::this_->RenderGUI();
        Graphics::this_->EndFrame();
    }

    void InternalApp::ViewChanged(int width, int height)
    {
        pApp_->ViewChanged(width, height);
    }

    void InternalApp::OnMouseMove(float x, float y)
    {
        pApp_->signalMouseMoved_->Run(x, y);
    }

    void InternalApp::OnMouseDown(int button, float x, float y)
    {
        pApp_->signalMouseDown_->Run(button, x, y);
    }

    void InternalApp::OnMouseUp(int button, float x, float y)
    {
        pApp_->signalMouseUp_->Run(button, x, y);
    }

    void InternalApp::OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        pApp_->signalMultiGesture_->Run(timestamp, x, y, dTheta, dDist, numFingers);
    }

    void InternalApp::OnMouseWheel(float x, float y)
    {
        pApp_->signalMouseWheel_->Run(x, y);
    }

    void InternalApp::OnKey(int key, int action, int modifier)
    {
        pApp_->signalKey_->Run(key, action, modifier);
    }

    void InternalApp::OnChar(unsigned int character)
    {
        pApp_->signalChar_->Run(character);
    }

    void InternalApp::RenderFrame()
    {
        PerformTicks();

        AppStatistics::this_->NewFrame();
    }

    bool InternalApp::ShallExit() const
    {
        return pApp_->ShallExit();
    }

    void InternalApp::AppEnterBackground()
    {
        pApp_->AppEnterBackground();
    }

    void InternalApp::AppEnterForeground()
    {
        pApp_->AppEnterForeground();
    }

    void InternalApp::InvalidateContext()
    {
        Context::this_->InvalidateObjects();
    }

    void InternalApp::HandleMessage(const pp::Var& var_message)
    {
        pApp_->HandleMessage(var_message);
    }

    void InternalApp::SetAssetManager(AAssetManager* pAAssetManager)
    {
        pApp_->SetAssetManager(pAAssetManager);
    }

    void InternalApp::SetActivity(ANativeActivity* pActivity)
    {
        Keyboard::this_->SetActivity(pActivity);
    }

    void InternalApp::DropFile(const std::string& filePath)
    {
        pApp_->DropFile(filePath);
    }

}