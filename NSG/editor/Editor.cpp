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
#include "Material.h"
#include "CameraControl.h"
#include "Keys.h"
#include "StringConverter.h"
#include "imgui.h"

namespace NSG
{
    Editor::Editor()
		: scenePreview_(std::make_shared<Scene>("NSGEditorPreview")),
          guiCapturedInput_(false),
		  editorCamera_(std::make_shared<Camera>())
    {
		CreatePreviewFrameBuffer();
        CreateEditorFrameBuffer();

		scenePreview_->CreateChild<Camera>();
		auto light = scenePreview_->CreateChild<Light>();
        light->EnableShadows(false);
        light->SetPosition(Vector3(5, 5, 15));
        auto sphere = Mesh::GetOrCreate<SphereMesh>("NSGEditorSphereMesh");
        sphere->Set(1, 32);
		previewNode_ = scenePreview_->CreateChild<SceneNode>();
        previewNode_->SetMesh(sphere);
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
                    ShowProperties();
					ShowScene();
                });
            }
            else
            {
                slotDrawGUI_ = nullptr;
            }
        }
    }

    void Editor::SetSceneNode(PSceneNode node)
    {
        node_ = node;
    }

    void Editor::CreatePreviewFrameBuffer()
    {
        FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
        previewFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("PreviewFrameBuffer"), frameBufferFlags);
        previewFrameBuffer_->SetSize(128, 128);
        //previewFrameBuffer_->GetColorTexture()->SetUVTransform(Vector4(1, -1, 0, 0)); //In order to flip Y
    }

    void Editor::CreateEditorFrameBuffer()
    {
        FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
        editorFrameBuffer_ = std::make_shared<FrameBuffer>(GetUniqueName("EditorFrameBuffer"), frameBufferFlags);
		editorFrameBuffer_->SetSize(128, 128);
    }

    void Editor::OnMouseDown(int button, float x, float y)
    {
        if (guiCapturedInput_)
            return;

        if (button == NSG_BUTTON_RIGHT && control_)
        {
            auto node = control_->SelectObject();
            if(node)
                SetSceneNode(node);
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

		if (ImGui::Begin("Scene", &opened, ImVec2(285, 414), bg_alpha, window_flags))
		{
			auto texture = GetScenePreview(scene.get(), camera.get());
			if (texture->IsReady())
				ImGui::Image((void*)texture->GetID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), ImVec2(0, 0), ImVec2(1, -1));

		}
		ImGui::End();
	}

	void Editor::ShowProperties()
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

        if (ImGui::Begin("Properties", &opened, ImVec2(285, 414), bg_alpha, window_flags))
        {
            auto node = node_.lock();
            if (node)
            {
                ShowSceneNode(node);
                ShowMaterial(node->GetMaterial());
            }
        }
        ImGui::End();

        ImGuiIO& io = ImGui::GetIO();
        guiCapturedInput_ = io.WantCaptureMouse || io.WantCaptureKeyboard;
        if (control_)
            control_->Enable(!guiCapturedInput_);
    }

    void Editor::ShowSceneNode(PSceneNode node)
    {
        if (node)
        {
            std::string header = "Node:" + node->GetName();
            if (ImGui::CollapsingHeader(header.c_str()))
            {
                if (ImGui::TreeNode("Transform"))
                {
                    auto position = node->GetPosition();
                    ImGui::DragFloat3("Position", &position[0], 0.1f);
                    node->SetPosition(position);

                    auto guiRotation = node->GetGUIRotation();
                    auto oldRotation = Radians(guiRotation);
                    ImGui::DragFloat3("Rotation", &guiRotation[0], 1, 0, 360);
                    auto rad = Radians(guiRotation);
                    auto q = node->GetOrientation();
                    q *= Inverse(Quaternion(oldRotation)) * Quaternion(rad);
                    node->SetOrientation(q);
                    node->SetGUIRotation(guiRotation);

                    auto scale = node->GetScale();
                    ImGui::DragFloat3("Scale", &scale[0], 0.1f);
                    node->SetScale(scale);

                    ImGui::TreePop();
                }
            }
        }
    }

    void Editor::ShowMaterial(PMaterial material)
    {
        if (material)
        {
            std::string header = "Material:" + material->GetName();
            if (ImGui::CollapsingHeader(header.c_str()))
            {
                if (ImGui::TreeNode("Preview"))
                {
					auto texture = GetMaterialPreview(material);
                    if (texture->IsReady())
                        ImGui::Image((void*)texture->GetID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), ImVec2(0, 0), ImVec2(1, -1));
                    ImGui::TreePop();
                }

				if (material->HasTextures() && ImGui::TreeNode("Textures"))
                {
                    for(int i=0; i<(int)MaterialTexture::MAX_MAPS; i++)
                    {
                        auto texture = material->GetTexture((MaterialTexture)i);
                        if(texture && texture->IsReady())
                        {
                            auto type = texture->GetMapType();
							if (ImGui::TreeNode(ToString(type)))
                            {
								auto width = std::min(64.f, (float)texture->GetWidth());
								auto height = std::min(64.f, (float)texture->GetHeight());
								ImGui::Image((void*)texture->GetID(), ImVec2(width, height), ImVec2(0, 0), ImVec2(1, -1));
                                ImGui::TreePop();
                            }
                        }
                    }
                    ImGui::TreePop();
                }

                auto shadeless = material->IsShadeless();
                ImGui::Checkbox("Shadeless", &shadeless);
                material->SetShadeless(shadeless);
                ImGui::SameLine();

                auto isTransparent = material->IsTransparent();
                ImGui::Checkbox("Transparent", &isTransparent);
                material->EnableTransparent(isTransparent);

                ImGui::SameLine();
                auto isFlipped = material->IsYFlipped();
                ImGui::Checkbox("Flip Y", &isFlipped);
                material->FlipYTextureCoords(isFlipped);

                auto renderPass = material->GetRenderPass();
                ImGui::Combo("Render Pass", (int*)&renderPass, "Vertex Color\0Unlit\0Lit\0Text\0Blend\0Blur\0Wave\0Show Diffuse\0");
                material->SetRenderPass(renderPass);
                auto fillMode = material->GetFillMode();
                ImGui::Combo("Fill Mode", (int*)&fillMode, "Solid\0Wireframe\0");
                material->SetFillMode(fillMode);
                auto cullFaceMode = material->GetCullFaceMode();
                ImGui::Combo("Culling", (int*)&cullFaceMode, "Back\0Front\0Front and back\0Disabled");
                material->SetCullFaceMode(cullFaceMode);
                auto billboardType = material->GetBillboardType();
                ImGui::Combo("Billboard", (int*)&billboardType, "None\0Spherical\0Cylindrical\0");
                material->SetBillboardType(billboardType);

                auto ambientIntensity = material->GetAmbientIntensity();
                ImGui::SliderFloat("##ambInt", &ambientIntensity, 0.0f, 1.0f, "Ambient Intensity %.3f");
                material->SetAmbientIntensity(ambientIntensity);

                auto emitIntensity = material->GetEmitIntensity();
                ImGui::SliderFloat("##emitInt", &emitIntensity, 0.0f, 1.0f, "Emit Intensity %.3f");
                material->SetEmitIntensity(emitIntensity);

                auto diffuseColor = material->GetDiffuseColor();
                ImGui::ColorEdit4("Diffuse", &diffuseColor[0]);
                material->SetDiffuseColor(ColorRGB(diffuseColor));
                material->SetAlpha(diffuseColor.a);

                auto diffuseIntensity = material->GetDiffuseIntensity();
                ImGui::SliderFloat("##difInt", &diffuseIntensity, 0.0f, 1.0f, "Diffuse Intensity %.3f");
                material->SetDiffuseIntensity(diffuseIntensity);

                auto specularColor = material->GetSpecularColor();
                ImGui::ColorEdit4("Specular", &specularColor[0]);
                material->SetSpecularColor(ColorRGB(specularColor));
                material->SetAlphaForSpecular(specularColor.a);

                auto specularIntensity = material->GetSpecularIntensity();
                ImGui::SliderFloat("##speInt", &specularIntensity, 0.0f, 1.0f, "Specular Intensity %.3f");
                material->SetSpecularIntensity(specularIntensity);

                auto shininess = material->GetShininess();
                ImGui::SliderFloat("##shinin", &shininess, 0.0f, 511.0f, "Specular Shininess %.0f");
                material->SetShininess(shininess);

                auto friction = material->GetFriction();
                ImGui::SliderFloat("##rbFric", &friction, 0.0f, 100.0f, "Rigbody Friction %.1f");
                material->SetFriction(friction);

                auto castShadow = material->CastShadow();
                ImGui::Checkbox("Cast Shadow", &castShadow);
                material->CastShadow(castShadow);
                ImGui::SameLine();

                auto receiveShadows = material->ReceiveShadows();
                ImGui::Checkbox("Receive Shadows", &receiveShadows);
                material->ReceiveShadows(receiveShadows);

                auto shadowBias = material->GetBias();
                ImGui::SliderFloat("##bias", &shadowBias, 0.0f, 10.0f, "Shadow Bias %.3f");
                material->SetBias(shadowBias);
            }
        }
    }

    PTexture Editor::GetMaterialPreview(PMaterial material)
    {
        if (previewFrameBuffer_->IsReady())
        {
            auto graphics = Graphics::GetPtr();
            auto currentFB = graphics->GetFrameBuffer();
            auto angle = 0.2f * Engine::GetPtr()->GetDeltaTime();
            auto q = AngleAxis(angle, Vertex3(0, 1, 0));
            auto rot = previewNode_->GetOrientation();
            previewNode_->SetOrientation(q * rot);

            previewNode_->SetMaterial(material);
            graphics->SetFrameBuffer(previewFrameBuffer_.get());
            auto currentWindow = graphics->GetWindow();
			Renderer::GetPtr()->Render(nullptr, scenePreview_.get());
            graphics->SetFrameBuffer(currentFB);
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
			auto currentFB = graphics->GetFrameBuffer();
			graphics->SetFrameBuffer(editorFrameBuffer_.get());
			auto currentWindow = graphics->GetWindow();
			Renderer::GetPtr()->Render(nullptr, scene, camera);
			graphics->SetFrameBuffer(currentFB);
			graphics->SetWindow(currentWindow);
			return editorFrameBuffer_->GetColorTexture();
		}
		return nullptr;
	}


}