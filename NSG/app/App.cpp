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
#include "Music.h"
#include "Program.h"
#include "RigidBody.h"
#include "ResourceFileManager.h"
#include "TextureFileManager.h"
#include "Object.h"
#include "Keyboard.h"
#include "Audio.h"
#include "AppStatistics.h"
#include "Check.h"
#include "Texture.h"
#include "UTF8String.h"
#include "SDL.h"
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

    App::App()
        : configuration_(new AppConfiguration),
          keyboard_(new Keyboard),
          nWindows2Remove_(0),
          mainWindow_(nullptr)
    {
        CHECK_ASSERT(SDL_WasInit(SDL_INIT_VIDEO) == 0, __FILE__, __LINE__);

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE))
            TRACE_LOG("SDL_Init Error: " << SDL_GetError() << std::endl);

        SDL_GL_SetSwapInterval(configuration_->vertical_sync_ ? 1 : 0);

        basePath_ = Path::GetCurrentDir();

        TRACE_LOG("Base path is: " << basePath_);

        resourceFileManager_ = PResourceFileManager(new ResourceFileManager);

        textureFileManager_ = PTextureFileManager(new TextureFileManager);

        audio_ = PAudio(new Audio);
    }

    App::~App()
    {
        ClearAll();
        Singleton<App>::this_ = nullptr;
    }

    void App::InitializeGraphics()
    {
        if (!graphics_)
        {
            graphics_ = PGraphics(new Graphics);
            statistics_ = PAppStatistics(new AppStatistics);
            graphics_->InitializeBuffers();
        }
    }

    void App::ClearAll()
    {
        resourceFileManager_ = nullptr;
        textureFileManager_ = nullptr;
        scenes_.Clear();
        meshes_.Clear();
        materials_.Clear();
        programs_.Clear();
        whiteTexture_ = nullptr;
        statistics_ = nullptr;
        graphics_ = nullptr;
    }


    void App::AddObject(Object* object)
    {
        CHECK_CONDITION(objects_.insert(object).second, __FILE__, __LINE__);
    }

    void App::RemoveObject(Object* object)
    {
        CHECK_CONDITION(objects_.erase(object), __FILE__, __LINE__);
    }

    void App::InvalidateObjects()
    {
        TRACE_LOG("App::InvalidateObjects...");

        for (auto& obj : objects_)
            obj->Invalidate();

        graphics_->ResetCachedState();

        TRACE_LOG("App::InvalidateObjects done");
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
            whiteTexture_ = PTexture(new Texture(GL_RGB, WIDTH, HEIGHT, (char*)&img[0]));
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

    PScene App::GetOrCreateScene(const std::string& name)
    {
        return scenes_.GetOrCreate(name);
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
        return resourceFileManager_->GetOrCreate(path);
    }

    PTexture App::GetOrCreateTextureFile(const Path& path, TextureFlags flags)
    {
        return TextureFileManager::this_->GetOrCreate(path);
    }

    PProgram App::GetOrCreateProgram(const std::string& name)
    {
        return programs_.GetOrCreate(name);
    }

	PTextMesh App::CreateTextMesh(const std::string& name, PFontAtlasTexture atlas, bool dynamic)
    {
        PTextMesh mesh = meshes_.CreateClass<TextMesh>(name);
		mesh->SetAtlas(atlas);
        mesh->SetDynamic(dynamic);
        return mesh;
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

    void App::SetMainWindow(Window* window)
    {
        if (!mainWindow_)
            mainWindow_ = window;
    }

    void App::RenderFrame(void* data)
    {
        App* pThis = (App*)data;

        #if IOS
        if (pThis->mainWindow_->IsClosed())
        {
            SDL_Quit();
            exit(0); //force quit on IOS
        }
        #endif

        if (!pThis->mainWindow_->IsMinimized())
        {
            pThis->resourceFileManager_->IsReady();
            #if defined(EMSCRIPTEN) || defined(IOS)
            pThis->HandleEvents();
            #endif
            for (auto& obj : pThis->windows_)
            {
                PWindow window(obj.lock());
                if (!window || window->IsClosed())
                    break;
                window->RenderFrame();
            }
        }
        else
        {
            std::this_thread::sleep_for(Milliseconds(1000));
        }
    }

    int App::Run()
    {
        if (windows_.empty())
            return 0;

        #if IOS
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
            while (windows_.size())
            {
                SDL_GL_MakeCurrent(mainWindow_->GetSDLWindow(), mainWindow_->GetSDLContext());
                HandleEvents();
                App::RenderFrame(this);
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
            }
        }
        #endif

        return 0;
    }

    void App::HandleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            #if !defined(EMSCRIPTEN)
            auto it = std::find_if(windows_.begin(), windows_.end(), [&](PWeakWindow window) { return window.lock() && SDL_GetWindowID(window.lock()->GetSDLWindow()) == event.window.windowID; });
            if (it == windows_.end())
                continue;
            PWindow window(it->lock());
            if (!window)
                continue;
            int width;
            int height;
            SDL_GetWindowSize(window->GetSDLWindow(), &width, &height);
            #else
            Window* window = mainWindow_;
            int width = window->GetWidth();
            int height = window->GetHeight();
            #endif

            if (event.type == SDL_WINDOWEVENT)
            {
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

                        #if !EMSCRIPTEN
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_RESTORED:
                        {
                            window->EnterForeground();
                            window->ViewChanged(width, height);
                            break;
                        }
                        #endif
                    default:
                        break;
                }
            }
            #if EMSCRIPTEN
            else if (event.type == SDL_VIDEORESIZE)
            {
                SDL_ResizeEvent* r = (SDL_ResizeEvent*)&event;
                int width = r->w;
                int height = r->h;
                window->ViewChanged(width, height);
            }
            #else
            else if (event.type == SDL_APP_DIDENTERBACKGROUND)
            {
                window->EnterBackground();
            }
            else if (event.type == SDL_APP_DIDENTERFOREGROUND)
            {
                window->EnterForeground();
            }
            else if (event.type == SDL_DROPFILE)
            {
                window->DropFile(event.drop.file);
                SDL_free(event.drop.file);
            }
            #endif
            else if (event.type == SDL_QUIT)
            {
                window->Close();
            }
            else if (event.type == SDL_KEYDOWN)
            {
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
                int key = event.key.keysym.sym;
                //int scancode = event.key.keysym.scancode;
                int action = NSG_KEY_RELEASE;
                int modifier = event.key.keysym.mod;
                window->OnKey(key, action, modifier);
            }
            else if (event.type == SDL_TEXTINPUT)
            {
                #ifndef __GNUC__
                {
                    std::string utf8(event.text.text);
                    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
                    std::u16string utf16 = utf16conv.from_bytes(utf8);
                    for (char16_t c : utf16)
                    {
                        window->OnChar((unsigned int)c);
                    }
                }
                #else
                {
                    UTF8String utf8(event.text.text);
                    unsigned unicode = utf8.AtUTF8(0);
                    if (unicode)
                    {
                        window->OnChar(unicode);
                    }
                }
                #endif
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                float x = (float)event.button.x;
                float y = (float)event.button.y;
                window->OnMouseDown(event.button.button, -1 + 2 * x / width, 1 + -2 * y / height);
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                float x = (float)event.button.x;
                float y = (float)event.button.y;
                window->OnMouseUp(event.button.button, -1 + 2 * x / width, 1 + -2 * y / height);
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                if (width > 0 && height > 0)
                {
                    float x = (float)event.motion.x;
                    float y = (float)event.motion.y;
                    window->OnMouseMove(-1 + 2 * x / width, 1 + -2 * y / height);
                }
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                window->OnMouseWheel((float)event.wheel.x, (float)event.wheel.y);
            }
            else if (event.type == SDL_FINGERDOWN)
            {
                float x = event.tfinger.x;
                float y = event.tfinger.y;
                window->OnMouseDown(0, -1 + 2 * x, 1 + -2 * y);
            }
            else if (event.type == SDL_FINGERUP)
            {
                float x = event.tfinger.x;
                float y = event.tfinger.y;
                window->OnMouseUp(0, -1 + 2 * x, 1 + -2 * y);
            }
            else if (event.type == SDL_FINGERMOTION)
            {
                float x = event.tfinger.x;
                float y = event.tfinger.y;
                window->OnMouseMove(-1 + 2 * x, 1 + -2 * y);
            }
            #if !defined(EMSCRIPTEN)
            else if (event.type == SDL_MULTIGESTURE)
            {
                float x = event.mgesture.x;
                float y = event.mgesture.y;

                window->OnMultiGesture(event.mgesture.timestamp, -1 + 2 * x, 1 + -2 * y, event.mgesture.dTheta, event.mgesture.dDist, (int)event.mgesture.numFingers);
            }
            #endif
        }
    }
}

