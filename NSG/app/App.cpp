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
#if NACL
#include "ppapi/cpp/var.h"
#endif

namespace NSG
{
    template <> App* Singleton<App>::this_ = nullptr;

    App::App()
        : width_(0),
          height_(0),
          argc_(0),
          argv_(nullptr)
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
        currentScene_ = nullptr;
        scenes_.clear();
        meshes_.clear();
        materials_.clear();
        App::this_ = nullptr;
        TRACE_LOG("App Terminated");
    }

    void App::Update()
    {
        currentScene_->Update();
    }

    void App::RenderFrame()
    {
        currentScene_->Render();
    }

    void App::ViewChanged(int width, int height)
    {
        currentScene_->ViewChanged(width, height);
    }

    void App::OnMouseMove(float x, float y)
    {
        currentScene_->OnMouseMove(x, y);
    }

	void App::OnMouseDown(int button, float x, float y)
    {
        currentScene_->OnMouseDown(button, x, y);
    }

    void App::OnMouseWheel(float x, float y)
    {
        currentScene_->OnMouseWheel(x, y);
    }

	void App::OnMouseUp(int button, float x, float y)
    {
        currentScene_->OnMouseUp(button, x, y);
    }

    void App::OnKey(int key, int action, int modifier)
    {
        currentScene_->OnKey(key, action, modifier);
    }

    void App::OnChar(unsigned int character)
    {
        currentScene_->OnChar(character);
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

    void App::DropFile(const std::string& filename)
    {
        TRACE_LOG("Dropped file:" << filename);
    }

    void App::SetCommandLineParameters(int argc, char* argv[])
    {
        argc_ = argc;
        argv_ = argv;
    }

    void App::SetViewSize(int width, int height)
    {
        TRACE_LOG("SetViewSize: width=" << width << " height=" << height);
        
        width_ = width;
        height_ = height;

        for (auto& listener : viewChangedListeners_)
            listener->OnViewChanged(width, height);
    }

    std::pair<int, int> App::GetViewSize() const
    {
        return std::pair<int, int>(width_, height_);
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

    void App::AddListener(IViewChangedListener* listener)
    {
        viewChangedListeners_.push_back(listener);
        listener->OnViewChanged(width_, height_); //notify inmediately to have correct view size
    }

    void App::RemoveListener(IViewChangedListener* listener)
    {
        viewChangedListeners_.erase(std::find(viewChangedListeners_.begin(), viewChangedListeners_.end(), listener));
    }

    void App::Add(IViewChangedListener* listener)
    {
        if (App::this_)
        {
            App::this_->AddListener(listener);
        }
    }

    void App::Remove(IViewChangedListener* listener)
    {
        if (App::this_)
        {
            App::this_->RemoveListener(listener);
        }
    }

    PScene App::CreateScene(bool setAsCurrent)
    {
        PScene scene(new Scene);
        if(setAsCurrent)
            currentScene_ = scene;
        scenes_.push_back(scene);
        return scene;
    }

    void App::SetCurrentScene(PScene scene)
    {
        CHECK_ASSERT(scene, __FILE__, __LINE__);
        currentScene_ = scene;
    }

    PScene App::GetCurrentScene() const
    {
        return currentScene_;
    }

    PBoxMesh App::CreateBoxMesh(float width, float height, float depth, int resX, int resY, int resZ)
    {
        PBoxMesh mesh(new BoxMesh(width, height, depth, resX, resY, resZ));
        meshes_.push_back(mesh);
        return mesh;
    }

    PCircleMesh App::CreateCircleMesh(float radius, int res)
    {
        PCircleMesh mesh(new CircleMesh(radius, res));
        meshes_.push_back(mesh);
        return mesh;
    }

    PEllipseMesh App::CreateEllipseMesh(float width, float height, int res)
    {
        PEllipseMesh mesh(new EllipseMesh(width, height, res));
        meshes_.push_back(mesh);
        return mesh;
    }

    PModelMesh App::CreateModelMesh(const VertexsData& vertexsData, const Indexes& indexes)
    {
        PModelMesh mesh(new ModelMesh(vertexsData, indexes));
        meshes_.push_back(mesh);
        return mesh;
    }

    PModelMesh App::CreateModelMesh()
    {
        PModelMesh mesh(new ModelMesh);
        meshes_.push_back(mesh);
        return mesh;
    }


    PPlaneMesh App::CreatePlaneMesh(float width, float height, int columns, int rows)
    {
        PPlaneMesh mesh(new PlaneMesh(width, height, columns, rows));
        meshes_.push_back(mesh);
        return mesh;
    }

    PRectangleMesh App::CreateRectangleMesh(float width, float height)
    {
        PRectangleMesh mesh(new RectangleMesh(width, height));
        meshes_.push_back(mesh);
        return mesh;
    }

    PRoundedRectangleMesh App::CreateRoundedRectangleMesh(float radius, float width, float height, int res)
    {
        PRoundedRectangleMesh mesh(new RoundedRectangleMesh(radius, width, height, res));
        meshes_.push_back(mesh);
        return mesh;
    }

    PSphereMesh App::CreateSphereMesh(float radius, int res)
    {
        PSphereMesh mesh(new SphereMesh(radius, res));
        meshes_.push_back(mesh);
        return mesh;
    }

    PTextMesh App::CreateTextMesh(const std::string& textureFilename, bool dynamic)
    {
        PTextMesh mesh(new TextMesh(textureFilename, dynamic));
        meshes_.push_back(mesh);
        return mesh;
    }

    PMaterial App::CreateMaterial(const std::string& name)
    {
        PMaterial material(new Material(name));
        materials_.push_back(material);
        return material;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    InternalApp::InternalApp(App* pApp)
        : Tick(pApp->configuration_->fps_),
          pApp_(pApp),
          screenX_(0),
          screenY_(0)
    {
    }

    InternalApp::~InternalApp()
    {
        pApp_ = nullptr;
    }

    void InternalApp::InitializeTicks()
    {
        Context::this_->Initialize();

        PScene scene = pApp_->CreateScene(true);

        pApp_->Start(pApp_->argc_, pApp_->argv_);

        scene->Start();
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
#if 1
        IMGUI::Context::this_->RenderGUI();
#endif

        Graphics::this_->EndFrame();
    }

    void InternalApp::SetViewSize(int width, int height)
    {
        pApp_->SetViewSize(width, height);
    }

    void InternalApp::ViewChanged(int width, int height)
    {
        pApp_->SetViewSize(width, height);
        pApp_->ViewChanged(width, height);
    }

    void InternalApp::OnMouseMove(float x, float y)
    {
        screenX_ = x;
        screenY_ = y;

        IMGUI::OnMouseMove(x, y);
        pApp_->OnMouseMove(x, y);
    }

	void InternalApp::OnMouseDown(int button, float x, float y)
    {
        screenX_ = x;
        screenY_ = y;

        IMGUI::OnMouseDown(button, x, y);
        pApp_->OnMouseDown(button, x, y);
    }

	void InternalApp::OnMouseUp(int button, float x, float y)
    {
        IMGUI::OnMouseUp(button, x, y);
        pApp_->OnMouseUp(button, x, y);
    }

    void InternalApp::OnMouseWheel(float x, float y)
    {
        IMGUI::OnMouseWheel(x, y);
        pApp_->OnMouseWheel(x, y);
    }

    void InternalApp::OnKey(int key, int action, int modifier)
    {
        //TRACE_LOG("key=" << key << " action=" << action << " modifier=" << modifier);
        IMGUI::OnKey(key, action, modifier);
        pApp_->OnKey(key, action, modifier);
    }

    void InternalApp::OnChar(unsigned int character)
    {
        //TRACE_LOG("character=" << character);
        IMGUI::OnChar(character);
        pApp_->OnChar(character);
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

    void InternalApp::ReleaseResourcesFromMemory()
    {
        Context::this_->ReleaseResourcesFromMemory();
    }

    void InternalApp::InvalidateGPUContext()
    {
        Context::this_->InvalidateGPUResources();
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

    void InternalApp::DropFile(const std::string& filename)
    {
        pApp_->DropFile(filename);
    }

}