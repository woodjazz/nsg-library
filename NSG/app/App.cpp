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
#include "SDLWindow.h"
#include "Log.h"
#include "Keys.h"
#include "AppConfiguration.h"
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
#include "Technique.h"
#include "Music.h"
#include "Program.h"
#include "RigidBody.h"
#include "Object.h"
#include "Keyboard.h"
#include "Audio.h"
#include "Check.h"
#include "Texture.h"
#include "ResourceFile.h"
#include "ResourceMemory.h"
#include "UTF8String.h"
#include "SDL.h"
#include "pugixml.hpp"
#undef main

#include <algorithm>
#include <sstream>
#include <thread>
#ifndef __GNUC__
#include <codecvt>
#endif

#if EMSCRIPTEN
#include <emscripten.h>
#endif

namespace NSG
{
    template <> App* Singleton<App>::this_ = nullptr;

    #if defined(IOS) || defined(ANDROID)

    extern void AtExit();
    int ExitEventFilter(void* userdata, SDL_Event* event)
    {
        if (event->type == SDL_APP_TERMINATING)
        {
            AtExit();
            SDL_Quit();
            std::exit(0);
            return 0;
        }
        return 1;
    }

    #endif

    App::App()
        : configuration_(new AppConfiguration),
          keyboard_(new Keyboard),
          nWindows2Remove_(0),
          mainWindow_(nullptr)
    {
        #if EMSCRIPTEN
        int flags = 0;
        #else
        int flags = SDL_INIT_EVENTS;
        #endif

        if (SDL_Init(flags))
            TRACE_LOG("SDL_Init Error: " << SDL_GetError() << std::endl);

        #if defined(IOS) || defined(ANDROID)
        {
            SDL_AddEventWatch(ExitEventFilter, nullptr);
        }
        #endif
    }

    App::~App()
    {
        TRACE_PRINTF("Closing App...");
        ClearAll();
        Singleton<App>::this_ = nullptr;
    }

    void App::InitializeGraphics()
    {
        std::call_once(onceFlag_, [&]()
        {
            CHECK_ASSERT(!graphics_, __FILE__, __LINE__);
            graphics_ = PGraphics(new Graphics);
        });
    }

    void App::ClearAll()
    {
        meshes_.Clear();
        materials_.Clear();
        resources_.Clear();
        whiteTexture_ = nullptr;
        graphics_ = nullptr;
    }


    void App::AddObject(Object* object)
    {
        if (App::this_)
            CHECK_CONDITION(objects_.insert(object).second, __FILE__, __LINE__);
    }

    void App::RemoveObject(Object* object)
    {
        if (App::this_)
            CHECK_CONDITION(objects_.erase(object), __FILE__, __LINE__);
    }

    PTexture App::GetWhiteTexture()
    {
        if (!whiteTexture_)
        {
            const int WIDTH = 1;
            const int HEIGHT = 1;
            // Creates 1x1 white texture
            static unsigned char img[WIDTH * HEIGHT * 3];
            memset(&img[0], 0xFF, sizeof(img));
            whiteTexture_ = std::make_shared<Texture>("WhiteTexture", GL_RGB, WIDTH, HEIGHT, (char*)&img[0]);
            TRACE_LOG("White texture has been generated.");
        }

        return whiteTexture_;
    }


    PWindow App::GetOrCreateWindow(const std::string& name, int x, int y, int width, int height)
    {
        #if defined(IS_TARGET_MOBILE) || defined(IS_TARGET_WEB)
        {
            if (windows_.size())
            {
                TRACE_LOG("Only one window is allowed for this platform!!!");
                return nullptr;
            }
        }
        #endif

        auto window = PWindow(new SDLWindow(name, x, y, width, height));
        windows_.push_back(window);
        return window;
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

	PModelMesh App::GetModelMesh(const std::string& name) const
	{
		return meshes_.GetClass<ModelMesh>(name);
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

    PMaterial App::CreateMaterial(const std::string& name, const ProgramFlags& flags)
    {
        auto material = materials_.Create(name);
		if ((int)ProgramFlag::NONE != flags)
            material->SetProgramFlags(0, flags);
        return material;
    }

    PMaterial App::GetOrCreateMaterial(const std::string& name, const ProgramFlags& flags)
    {
        auto material = materials_.GetOrCreate(name);
		if ((int)ProgramFlag::NONE != flags)
            material->SetProgramFlags(0, flags);
        return material;
    }

    PMaterial App::GetMaterial(const std::string& name)
    {
        return materials_.Get(name);
    }   

    PResourceFile App::GetOrCreateResourceFile(const std::string& name)
    {
		return resources_.GetOrCreateClass<ResourceFile>(name);
    }

    PResourceMemory App::GetOrCreateResourceMemory(const std::string& name)
    {
		return resources_.GetOrCreateClass<ResourceMemory>(name);
    }

	PResource App::GetResource(const std::string& name)
	{
		return resources_.Get(name);
	}

    std::vector<PMesh>& App::GetMeshes()
    {
        return meshes_.GetObjs();
    }

	const std::vector<PMesh>& App::GetConstMeshes() const
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

    const std::vector<PResource>& App::GetResources() const
    {
        return resources_.GetConstObjs();
    }

    void App::SetMainWindow(Window* window)
    {
        if (!mainWindow_)
            mainWindow_ = window;
        else if (!window)
            mainWindow_ = nullptr;
    }

    bool App::RenderFrame()
    {
        HandleEvents();
        for (auto& obj : windows_)
        {
            PWindow window(obj.lock());
            if (!window || window->IsClosed())
                break;
            if (!window->IsMinimized())
                window->RenderFrame();
        }

        while (nWindows2Remove_)
        {
            windows_.erase(std::remove_if(windows_.begin(), windows_.end(), [&](PWeakWindow window)
            {
                if (!window.lock() || window.lock()->IsClosed())
                {
                    --nWindows2Remove_;
                    return true;
                }
                return false;
            }), windows_.end());
        }

        if (!mainWindow_)
            return false;
        else if (mainWindow_->IsMinimized())
        {
            std::this_thread::sleep_for(Milliseconds(100));
        }
        return true;
    }

    void App::RenderFrame(void* data)
    {
        App* pThis = (App*)data;
        //#if !defined(IOS) && !defined(EMSCRIPTEN)
        #if !defined(EMSCRIPTEN)
        {
            while (pThis->RenderFrame());
        }
        #else
        {
            pThis->RenderFrame();
        }
        #endif

    }

    int App::Run()
    {
        #if 0//IOS
        {
            SDL_iPhoneSetAnimationCallback(mainWindow_->GetSDLWindow(), 1, &RenderFrame, this);

        }
        #elif EMSCRIPTEN
        {
            SDL_StartTextInput();
            emscripten_set_main_loop_arg(&RenderFrame, this, 0, 1);
            emscripten_run_script("setTimeout(function() { window.close() }, 2000)");
        }
        #else
        {
            App::RenderFrame(this);
        }
        #endif

        return 0;
    }

    bool App::GetWindow(unsigned windowID, Window*& window, int& width, int& height)
    {
        window = nullptr;
        #if defined(EMSCRIPTEN)
        {
            int isFullscreen;
            emscripten_get_canvas_size(&width, &height, &isFullscreen);
            window = mainWindow_;
        }
        #elif defined(IS_TARGET_MOBILE)
        {
            window = mainWindow_;
            SDL_GetWindowSize(window->GetSDLWindow(), &width, &height);
        }
        #else
        {
            auto it = std::find_if(windows_.begin(), windows_.end(), [&](PWeakWindow win)
            {
                auto w = win.lock();
                return w && SDL_GetWindowID(w->GetSDLWindow()) == windowID;
            });

            if (it != windows_.end())
            {
                window = it->lock().get();
                SDL_GetWindowSize(window->GetSDLWindow(), &width, &height);
            }
        }
        #endif
        return window != nullptr;
    }

    bool App::GetWindow(unsigned windowID, Window*& window)
    {
        window = nullptr;
        #if defined(IS_TARGET_MOBILE) || defined(IS_TARGET_WEB)
        {
            window = mainWindow_;
        }
        #else
        {
            auto it = std::find_if(windows_.begin(), windows_.end(), [&](PWeakWindow win)
            {
                auto w = win.lock();
                return w && SDL_GetWindowID(w->GetSDLWindow()) == windowID;
            });

            if (it != windows_.end())
                window = it->lock().get();
        }
        #endif

        return window != nullptr;
    }

    void App::HandleEvents()
    {
        Window* window;
        int width;
        int height;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_WINDOWEVENT)
            {
                if (!GetWindow(event.window.windowID, window, width, height))
                    continue;

                window->ViewChanged(width, height);
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE:
                        window->Close();
                        break;
                    case SDL_WINDOWEVENT_MINIMIZED:
                        window->EnterBackground();
                        break;
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        window->EnterForeground();
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        //window->ViewChanged(width, height);
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        window->EnterForeground();
                        //window->ViewChanged(width, height);
                        break;
                    default:
                        break;
                }
            }
            #if 0
            else if (event.type == SDL_APP_DIDENTERBACKGROUND)
            {
                window->EnterBackground();
            }
            else if (event.type == SDL_APP_DIDENTERFOREGROUND)
            {
                window->EnterForeground();
            }
            else if (event.type == SDL_QUIT)
            {
                window->Close();
            }
            else if (event.type == SDL_DROPFILE)
            {
                window->DropFile(event.drop.file);
                SDL_free(event.drop.file);
            }
            #endif
            else if (event.type == SDL_KEYDOWN)
            {
                if (!GetWindow(event.key.windowID, window))
                    continue;
                int key = event.key.keysym.sym;
                #if ANDROID
                {
                    if (key == SDLK_AC_BACK)
                        window->Close();
                }
                #endif

                //int scancode = event.key.keysym.scancode;
                int action = NSG_KEY_PRESS;
                int modifier = event.key.keysym.mod;
                window->OnKey(key, action, modifier);
            }
            else if (event.type == SDL_KEYUP)
            {
                if (!GetWindow(event.key.windowID, window))
                    continue;
                int key = event.key.keysym.sym;
                //int scancode = event.key.keysym.scancode;
                int action = NSG_KEY_RELEASE;
                int modifier = event.key.keysym.mod;
                window->OnKey(key, action, modifier);
            }
            else if (event.type == SDL_TEXTINPUT)
            {
                if (!GetWindow(event.key.windowID, window))
                    continue;
                UTF8String utf8(event.text.text);
                unsigned unicode = utf8.AtUTF8(0);
                if (unicode)
                    window->OnChar(unicode);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (!GetWindow(event.key.windowID, window, width, height))
                    continue;
                float x = (float)event.button.x;
                float y = (float)event.button.y;
                window->OnMouseDown(event.button.button, -1 + 2 * x / width, 1 + -2 * y / height);
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (!GetWindow(event.key.windowID, window, width, height))
                    continue;
                float x = (float)event.button.x;
                float y = (float)event.button.y;
                window->OnMouseUp(event.button.button, -1 + 2 * x / width, 1 + -2 * y / height);
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                if (!GetWindow(event.key.windowID, window, width, height))
                    continue;
                if (width > 0 && height > 0)
                {
                    float x = (float)event.motion.x;
                    float y = (float)event.motion.y;
                    window->OnMouseMove(-1 + 2 * x / width, 1 + -2 * y / height);
                }
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                if (!GetWindow(event.key.windowID, window))
                    continue;
                window->OnMouseWheel((float)event.wheel.x, (float)event.wheel.y);
            }
            else if (event.type == SDL_FINGERDOWN)
            {
                if (!GetWindow(0, window))
                    continue;
                float x = event.tfinger.x;
                float y = event.tfinger.y;
                window->OnMouseDown(0, -1 + 2 * x, 1 + -2 * y);
            }
            else if (event.type == SDL_FINGERUP)
            {
                if (!GetWindow(0, window))
                    continue;
                float x = event.tfinger.x;
                float y = event.tfinger.y;
                window->OnMouseUp(0, -1 + 2 * x, 1 + -2 * y);
            }
            else if (event.type == SDL_FINGERMOTION)
            {
                if (!GetWindow(0, window))
                    continue;
                float x = event.tfinger.x;
                float y = event.tfinger.y;
                window->OnMouseMove(-1 + 2 * x, 1 + -2 * y);
            }
            #if !defined(EMSCRIPTEN)
            else if (event.type == SDL_MULTIGESTURE)
            {
                if (!GetWindow(0, window))
                    continue;
                float x = event.mgesture.x;
                float y = event.mgesture.y;

                window->OnMultiGesture(event.mgesture.timestamp, -1 + 2 * x, 1 + -2 * y, event.mgesture.dTheta, event.mgesture.dDist, (int)event.mgesture.numFingers);
            }
            #endif
        }
    }

    void App::LoadResources(const pugi::xml_node& node)
    {
        pugi::xml_node resources = node.child("Resources");
        if (resources)
        {
            pugi::xml_node child = resources.child("Resource");
            while (child)
            {
				auto resource = Resource::CreateFrom(child);
                resource->Load(child);
                child = child.next_sibling("Resource");
            }
        }
    }

    void App::LoadMeshes(const pugi::xml_node& node)
    {
        pugi::xml_node meshes = node.child("Meshes");
        if (meshes)
        {
            pugi::xml_node child = meshes.child("Mesh");
            while (child)
            {
                PModelMesh mesh(GetOrCreateModelMesh(child.attribute("name").as_string()));
                mesh->Load(child);
                child = child.next_sibling("Mesh");
            }
        }
    }

    void App::LoadMaterials(const pugi::xml_node& node)
    {
        pugi::xml_node objs = node.child("Materials");
		if (objs)
        {
			pugi::xml_node child = objs.child("Material");
            while (child)
            {
                PMaterial material(GetOrCreateMaterial(child.attribute("name").as_string()));
                material->Load(child);
                child = child.next_sibling("Material");
            }
        }
    }

    std::vector<PScene> App::Load(const pugi::xml_node& node)
    {
        LoadResources(node);
        LoadMeshes(node);
        LoadMaterials(node);
        std::vector<PScene> scenes;
        pugi::xml_node child = node.child("Scene");
        while (child)
        {
            auto scene = std::make_shared<Scene>("scene");
			scene->Load(child);
            scenes.push_back(scene);
            child = child.next_sibling("Scene");
        }
        return scenes;
    }

    std::vector<PScene> App::Load(PResource resource)
    {
        CHECK_CONDITION(resource->IsReady(), __FILE__, __LINE__);
        std::vector<PScene> scenes;
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer_inplace((void*)resource->GetData(), resource->GetBytes());
        if (result)
        {
            pugi::xml_node app = doc.child("App");
            if(app)
                scenes = Load(app);
        }
        else
        {
            TRACE_LOG("XML parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]");
            TRACE_LOG("Error description: " << result.description());
            TRACE_LOG("Error offset: " << result.offset << " (error at [..." << (result.offset) << "]");
            CHECK_ASSERT(false, __FILE__, __LINE__);
        }

        resource->Invalidate();
        return scenes;
    }

	pugi::xml_node App::SaveWithExternalResources(pugi::xml_document& doc, const Path& path, const Path& outputDir)
    {
        pugi::xml_node app = doc.append_child("App");
		SaveResourcesExternally(app, path, outputDir);
        SaveMeshes(app);
        SaveMaterials(app);
		return app;
    }

	pugi::xml_node App::Save(pugi::xml_document& doc)
	{
		pugi::xml_node app = doc.append_child("App");
		SaveResources(app);
		SaveMeshes(app);
		SaveMaterials(app);
		return app;
	}


	void App::SaveMeshes(pugi::xml_node& node) const
    {
        pugi::xml_node child = node.append_child("Meshes");
        auto meshes = GetConstMeshes();
        for (auto& obj : meshes)
            obj->Save(child);
    }

	void App::SaveMaterials(pugi::xml_node& node) const
    {
        pugi::xml_node child = node.append_child("Materials");
        auto materials = GetMaterials();
        for (auto& obj : materials)
            obj->Save(child);
    }

	void App::SaveResourcesExternally(pugi::xml_node& node, const Path& path, const Path& outputDir)
    {
        pugi::xml_node child = node.append_child("Resources");
        auto resources = GetResources();
        for (auto& obj : resources)
			obj->SaveExternal(child, path, outputDir);
    }

	void App::SaveResources(pugi::xml_node& node)
	{
		pugi::xml_node child = node.append_child("Resources");
		auto resources = GetResources();
		for (auto& obj : resources)
			obj->Save(child);
	}


    PTexture App::GetTextureWithResource(PResource resource) const
    {
		auto& materials = materials_.GetConstObjs();
		for (auto& material : materials)
        {
            auto texture = material->GetTextureWithResource(resource);
            if(texture)
                return texture;
        }

        return nullptr;
    }

}
