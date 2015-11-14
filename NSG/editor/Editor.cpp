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
		const FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
		previewFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("PreviewFrameBuffer"), frameBufferFlags);
		previewFrameBuffer_->SetSize(128, 128);
		sceneFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("SceneFrameBuffer"), frameBufferFlags);
		gameFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("GameFrameBuffer"), frameBufferFlags);

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
        auto window = window_.lock();
        if (window)
            window->RemoveEditor(this);
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
		{
			control_ = std::make_shared<CameraControl>(camera, scene);
			control_->SetSelectionContext(RendererContext::EDITOR);
		}
        else
            control_ = nullptr;
    }

    void Editor::SetWindow(PWindow window)
    {
        if (window != window_.lock())
        {
            auto oldWindow = window_.lock();
            if (oldWindow)
            {
                oldWindow->RemoveEditor(this);
            }
            window_ = window;
            if (window)
            {
				window->SetEditor(this);
                slotMouseDown_ = window->SigMouseDown()->Connect([this](int button, float x, float y)
                {
                    OnMouseDown(button, x, y);
                });
            }
            else
            {
                slotMouseDown_ = nullptr;
            }
        }
    }

    void Editor::Render()
    {
        auto window = window_.lock();
		editorGUI_.Render(window.get(), [this](){ShowWindows(); });
		RenderGame();
    }

    void Editor::ShowWindows()
    {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

        const ImGuiIO& io = ImGui::GetIO();
        auto mainWindowSize = io.DisplaySize;
        auto mainRectMin = ImGui::GetItemRectMin();

        const ImGuiWindowFlags hierarchyFlags = ImGuiWindowFlags_ShowBorders |
                                        ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoTitleBar;

		const float bg_alpha = -1;// 0.65f;

		if (ImGui::Begin("Hierarchy", nullptr, ImVec2(std::min(mainWindowSize.x * 0.5f, 320.f), mainWindowSize.y * 0.5f), bg_alpha, hierarchyFlags))
        {
            ImGui::SetWindowPos(mainRectMin);
            ShowHierachy();
        }
        auto hierarchyWindowPos = ImGui::GetWindowPos();
        auto hierarchyWindowSize = ImGui::GetWindowSize();
        ImGui::End();

		const ImGuiWindowFlags inspectorFlags = ImGuiWindowFlags_ShowBorders |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar;

		if (ImGui::Begin("Inspector", nullptr, ImVec2(0, 0), bg_alpha, inspectorFlags))
        {
            auto windowPos = hierarchyWindowPos;
            auto windowSize = hierarchyWindowSize;
            windowPos.y += windowSize.y;
            ImGui::SetWindowPos(windowPos);
            windowSize.y = mainWindowSize.y - windowPos.y;
            ImGui::SetWindowSize(windowSize);
            ShowInspector();
        }
        ImGui::End();

        {
			const ImGuiWindowFlags sceneFlags = ImGuiWindowFlags_ShowBorders |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoTitleBar;

			if (ImGui::Begin("Scene", nullptr, ImVec2(0, mainWindowSize.y * 0.5f), bg_alpha, sceneFlags))
            {
                auto windowPos = hierarchyWindowPos;
                auto windowSize = hierarchyWindowSize;
                windowPos.x += windowSize.x;
                ImGui::SetWindowPos(windowPos);
                windowSize.x = mainWindowSize.x - windowPos.x;
                windowSize.y = ImGui::GetWindowSize().y;
                ImGui::SetWindowSize(windowSize);
                ShowScene();
            }
            auto sceneWindowPos = ImGui::GetWindowPos();
            auto sceneWindowSize = ImGui::GetWindowSize();
            ImGui::End();

            {
				const ImGuiWindowFlags gameFlags = ImGuiWindowFlags_ShowBorders |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoTitleBar;
					//ImGuiWindowFlags_NoInputs; //(do not set this)

				if (ImGui::Begin("G", nullptr, ImVec2(0, 0), bg_alpha, gameFlags))
                {
                    auto windowPos = sceneWindowPos;
                    auto windowSize = sceneWindowSize;
                    windowPos.y += windowSize.y;
                    ImGui::SetWindowPos(windowPos);
                    windowSize.y = mainWindowSize.y - windowPos.y;
                    ImGui::SetWindowSize(windowSize);
					gameGUI_.SetArea(Rect{ windowPos.x, windowPos.y, windowSize.x, windowSize.y });
                    ShowGame();
                }
                ImGui::End();
            }
        }
        ImGui::PopStyleVar();
    }

    void Editor::SetNode(PNode node)
    {
        node_ = node;
    }

    Node* Editor::GetNode() const
    {
        auto p = node_.lock();
        if (p)
            return p.get();
        return nullptr;
    }

    void Editor::OnMouseDown(int button, float x, float y)
    {
        if (isSceneHovered_ && button == NSG_BUTTON_LEFT && control_)
        {
            auto node = control_->SelectObject(x, y);
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

        isSceneHovered_ = false;

        #if 1
        if (ImGui::TreeNode("Help"))
        {
			ImGui::BulletText("Click to select an object.\n");
			ImGui::BulletText("Press the F key over object.\nThis will set the pivot point on the selected point\nand center the Scene View.\n");
			ImGui::BulletText("Press the C key over object.\nThis will set the pivot point on the object's center\nand center the Scene View.\n");
			ImGui::BulletText("Hold Alt to orbit the camera around the current pivot point.\n");
			ImGui::BulletText("Hold Alt and Shift to zoom the Scene View.\nThis is the same as scrolling with your mouse wheel.\n");
			ImGui::BulletText("Hold Shift to drag camera around.\n");
			ImGui::BulletText("Press R to reset the camera.\n");
            ImGui::TreePop();
        }
        #endif

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));

        ImGui::BeginChild("", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
        auto texture = GetScenePreview(scene.get(), camera.get());
        Vector4 viewRect;
        if (texture && texture->IsReady())
        {
            ImGui::Image((void*)(intptr_t)texture->GetID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));

            auto vMin = ImGui::GetItemRectMin();
            viewRect[0] = vMin.x;
            viewRect[1] = vMin.y;

            auto vMax = ImGui::GetItemRectMax();
            viewRect[2] = vMax.x;
            viewRect[3] = vMax.y;

            isSceneHovered_ = ImGui::IsItemHovered();
        }
        ImGui::EndChild();

        ImGui::PopStyleVar();

        if (control_)
        {
            control_->Enable(isSceneHovered_);
            control_->SetViewRect(viewRect);
        }
    }

    void Editor::ShowGame()
    {
        auto graphics = Graphics::GetPtr();
        auto currentWindow = graphics->GetWindow();
        auto scene = currentWindow->GetScene();
        auto camera = scene->GetMainCamera();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
		auto size = ImGui::GetContentRegionAvail();
		if (size.x <= 0 || size.y <= 0)
			size = ImVec2{ 128, 128 };
		gameFrameBuffer_->SetSize((int)size.x, (int)size.y);

		if (gTexture_ && gTexture_->IsReady())
			ImGui::Image((void*)(intptr_t)gTexture_->GetID(), ImVec2((float)gTexture_->GetWidth(), (float)gTexture_->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::PopStyleVar();
    }

	void Editor::RenderGame()
	{
		auto graphics = Graphics::GetPtr();
		auto currentWindow = graphics->GetWindow();
		auto scene = currentWindow->GetScene();
		auto camera = scene->GetMainCamera();
		gTexture_ = GetGamePreview(scene, camera.get());
	}

	void Editor::ShowInspector()
    {
        //if (ImGui::CollapsingHeader("Inspector"))
        {
            auto node = node_.lock();
            if (node)
                node->ShowGUIProperties(this);
        }
    }

    void Editor::ShowHierachy()
    {
        //if (ImGui::CollapsingHeader("Hierachy"))
        {
            auto scene = scene_.lock();
            if (scene)
                scene->ShowGUIHierarchy(this);
        }
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
        if (size.x <= 0 || size.y <= 0)
            size = ImVec2{ 128, 128 };
        sceneFrameBuffer_->SetSize((int)size.x, (int)size.y);
        if (sceneFrameBuffer_->IsReady())
        {
            auto graphics = Graphics::GetPtr();
            auto oldFrameBuffer = graphics->SetFrameBuffer(sceneFrameBuffer_.get());
            auto currentWindow = graphics->GetWindow();
            auto renderer = Renderer::GetPtr();
            auto context = renderer->SetContext(RendererContext::EDITOR);
            renderer->Render(nullptr, scene, camera);
            renderer->SetContext(context);
            graphics->SetFrameBuffer(oldFrameBuffer);
            graphics->SetWindow(currentWindow);
            return sceneFrameBuffer_->GetColorTexture();
        }
        return nullptr;
    }

	PTexture Editor::GetGamePreview(Scene* scene, Camera* camera)
    {
        if (gameFrameBuffer_->IsReady())
        {
            auto graphics = Graphics::GetPtr();
            auto oldFrameBuffer = graphics->SetFrameBuffer(gameFrameBuffer_.get());
            auto currentWindow = graphics->GetWindow();
            auto renderer = Renderer::GetPtr();
            auto context = renderer->SetContext(RendererContext::DEFAULT);
            renderer->Render(nullptr, scene, camera);
			gameGUI_.Render(currentWindow, [&](){currentWindow->SigDrawIMGUI()->Run(); });
            renderer->SetContext(context);
            graphics->SetFrameBuffer(oldFrameBuffer);
            graphics->SetWindow(currentWindow);
            return gameFrameBuffer_->GetColorTexture();
        }
        return nullptr;
    }

}