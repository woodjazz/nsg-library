/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Editor.h"
#include "Window.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "SphereMesh.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Engine.h"
#include "Scene.h"
#include "Ray.h"
#include "Material.h"
#include "CameraControl.h"
#include "Keys.h"
#include "StringConverter.h"
#include "imgui.h"

namespace NSG
{
    Editor::Editor()
        : scenePreview_(std::make_shared<Scene>("NSGEditorPreview")),
          isSceneHovered_(false),
          editorCamera_(std::make_shared<Camera>("NSGEditorCamera"))
    {

        CreateEditorFrameBuffer();
        CreatePreviewFrameBuffer();

        scenePreview_->CreateChild<Camera>();
        auto light = scenePreview_->CreateChild<Light>();
        light->EnableShadows(false);
        light->SetPosition(Vector3(5, 5, 15));
        auto sphere = Mesh::GetOrCreate<SphereMesh>("NSGEditorSphereMesh");
        sphere->Set(1, 32);
        previewNode_ = scenePreview_->CreateChild<SceneNode>();
        previewNode_->SetPosition(Vector3(0, 0, -3));
        previewNode_->SetMesh(sphere);
        editorCamera_->SetFarClip(10000);
        editorCamera_->SetFOVDegrees(70);
        SetCamera(editorCamera_);
    }

    Editor::~Editor()
    {

    }

    void Editor::SetScene(PScene scene)
    {
        if (scene_.lock() != scene)
        {
            scene_ = scene;
            SetControl();
        }
    }

    void Editor::SetCamera(PCamera camera)
    {
        if (camera_.lock() != camera)
        {
            if (camera)
                camera_ = camera;
            else
                camera_ = editorCamera_;
            SetControl();
        }
    }

    void Editor::SetControl()
    {
        auto camera = camera_.lock();
        auto scene = scene_.lock();
        if (camera && scene)
            control_ = std::make_shared<CameraControl>(camera, scene);
        else
            control_ = nullptr;
    }

    void Editor::SetWindow(PWindow window)
    {
        if (window != window_.lock())
        {
            window_ = window;
            if (window)
            {
                slotMouseDown_ = window->SigMouseDown()->Connect([&](int button, float x, float y)
                {
                    OnMouseDown(button, x, y);
                });

                slotDrawGUI_ = window->SigDrawIMGUI()->Connect([this]()
                {
                    ShowInspector();
                    ShowScene();
                    ShowHierachy();
                });
            }
            else
            {
                slotDrawGUI_ = nullptr;
            }
        }
    }

    void Editor::SetNode(PNode node)
    {
        node_ = node;
    }

    void Editor::CreatePreviewFrameBuffer()
    {
        FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
        previewFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("PreviewFrameBuffer"), frameBufferFlags);
        previewFrameBuffer_->SetSize(128, 128);
        //previewFrameBuffer_->GetColorTexture()->SetWrapMode(TextureWrapMode::REPEAT);
        //previewFrameBuffer_->GetColorTexture()->SetUVTransform(Vector4(1, -1, 0, 0)); //In order to flip Y
    }

    void Editor::CreateEditorFrameBuffer()
    {
        FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
        editorFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("EditorFrameBuffer"), frameBufferFlags);
        editorFrameBuffer_->SetSize(128, 128);
        //editorFrameBuffer_->GetColorTexture()->SetWrapMode(TextureWrapMode::REPEAT);
    }

    void Editor::OnMouseDown(int button, float x, float y)
    {
        if (isSceneHovered_ && button == NSG_BUTTON_RIGHT && control_)
        {
            auto oldContext = Renderer::GetPtr()->SetContext(RendererContext::EDITOR);
            auto node = control_->SelectObject(x, y);
            Renderer::GetPtr()->SetContext(oldContext);
            if (node)
                SetNode(node);
        }
    }

    void Editor::ShowScene()
    {
        auto scene = scene_.lock();
        if (!scene)
            return;
        auto camera = camera_.lock();
        if (!camera)
            return;

        static bool no_titlebar = false;
        static bool no_border = false;
        static bool no_resize = false;
        static bool no_move = false;
        static bool no_scrollbar = true;
        static bool no_collapse = true;
        static bool no_menu = true;
        static bool no_autoresize = true;
        static float bg_alpha = 0.65f;
        static bool opened = true;

        ImGuiWindowFlags window_flags = 0;
        if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
        if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
        if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
        if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
        if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
        if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
        if (!no_autoresize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

        isSceneHovered_ = false;
        Vector4 viewRect;
        if (ImGui::Begin("Scene", &opened, ImVec2(285, 414), bg_alpha, window_flags))
        {
            auto texture = GetScenePreview(scene.get(), camera.get());
            if (texture && texture->IsReady())
            {
                ImGui::BeginChild("", ImVec2(0, 0), false, ImGuiWindowFlags_NoMove);
                ImGui::Image((void*)(intptr_t)texture->GetID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));

                auto vMin = ImGui::GetItemRectMin();
                viewRect[0] = vMin.x;
                viewRect[1] = vMin.y;

                auto vMax = ImGui::GetItemRectMax();
                viewRect[2] = vMax.x;
                viewRect[3] = vMax.y;

                isSceneHovered_ = ImGui::IsItemHovered();
                ImGui::EndChild();
            }
        }
        ImGui::End();
        if (control_)
        {
            control_->Enable(isSceneHovered_);
            control_->SetViewRect(viewRect);
        }
    }

    void Editor::ShowInspector()
    {
        static bool no_titlebar = false;
        static bool no_border = false;
        static bool no_resize = false;
        static bool no_move = false;
        static bool no_scrollbar = false;
        static bool no_collapse = true;
        static bool no_menu = false;
        static bool no_autoresize = false;
        static float bg_alpha = 0.65f;
        static bool opened = true;

        ImGuiWindowFlags window_flags = 0;
        if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
        if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
        if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
        if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
        if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
        if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
        if (!no_autoresize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::Begin("Inspector", &opened, ImVec2(285, 414), bg_alpha, window_flags))
        {
            auto node = node_.lock();
            if (node)
                node->ShowGUIProperties(this);
        }
        ImGui::End();
    }

    PTexture Editor::GetMaterialPreview(PMaterial material)
    {
        if (previewFrameBuffer_->IsReady())
        {
            auto graphics = Graphics::GetPtr();
            auto angle = 0.2f * Engine::GetPtr()->GetDeltaTime();
            auto q = AngleAxis(angle, Vertex3(0, 1, 0));
            auto rot = previewNode_->GetOrientation();
            previewNode_->SetOrientation(q * rot);
            previewNode_->SetMaterial(material);
            auto oldFrameBuffer = graphics->SetFrameBuffer(previewFrameBuffer_.get());
            auto currentWindow = graphics->GetWindow();
            Renderer::GetPtr()->Render(nullptr, scenePreview_.get());
            graphics->SetFrameBuffer(oldFrameBuffer);
            graphics->SetWindow(currentWindow);
            return previewFrameBuffer_->GetColorTexture();
        }
        return nullptr;
    }


    PTexture Editor::GetScenePreview(Scene* scene, Camera* camera)
    {
        auto size = ImGui::GetContentRegionAvail();
        editorFrameBuffer_->SetSize((int)size.x, (int)size.y);
        if (editorFrameBuffer_->IsReady())
        {
            auto graphics = Graphics::GetPtr();
            auto oldFrameBuffer = graphics->SetFrameBuffer(editorFrameBuffer_.get());
            auto currentWindow = graphics->GetWindow();
            auto renderer = Renderer::GetPtr();
            auto context = renderer->SetContext(RendererContext::EDITOR);
            renderer->Render(nullptr, scene, camera);
            renderer->SetContext(context);
            graphics->SetFrameBuffer(oldFrameBuffer);
            graphics->SetWindow(currentWindow);
            return editorFrameBuffer_->GetColorTexture();
        }
        return nullptr;
    }

    void Editor::ShowHierachy()
    {
        static bool no_titlebar = false;
        static bool no_border = false;
        static bool no_resize = false;
        static bool no_move = false;
        static bool no_scrollbar = false;
        static bool no_collapse = true;
        static bool no_menu = false;
        static bool no_autoresize = false;
        static float bg_alpha = 0.65f;
        static bool opened = true;

        ImGuiWindowFlags window_flags = 0;
        if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
        if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
        if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
        if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
        if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
        if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
        if (!no_autoresize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

        if (ImGui::Begin("Hierachy", &opened, ImVec2(285, 414), bg_alpha, window_flags))
        {
            auto scene = scene_.lock();
            if (scene)
                scene->ShowGUIHierarchy(this);
        }
        ImGui::End();
    }
}