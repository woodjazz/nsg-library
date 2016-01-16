/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "EditorSceneNode.h"
#include "NSG.h"

Editor::Editor()
	: scenePreview_(std::make_shared<Scene>("NSGEditorPreview")),
	isSceneHovered_(false),
	editorCamera_(std::make_shared<Camera>("NSGEditorCamera"))
{
	const FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH));
	previewFrameBuffer_ = PFrameBuffer(new FrameBuffer(GetUniqueName("PreviewFrameBuffer"), frameBufferFlags));
	previewFrameBuffer_->SetSize(128, 128);
	sceneFrameBuffer_ = PFrameBuffer(new FrameBuffer(GetUniqueName("SceneFrameBuffer"), frameBufferFlags));
	gameFrameBuffer_ = PFrameBuffer(new FrameBuffer(GetUniqueName("GameFrameBuffer"), frameBufferFlags));

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
        window->RemoveRender(this);
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
            oldWindow->RemoveRender(this);
        }
        window_ = window;
        if (window)
        {
			window->SetRender(this);
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
    auto graphics = RenderingContext::GetPtr();
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
	auto graphics = RenderingContext::GetPtr();
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
		{
			auto p = dynamic_cast<Camera*>(node.get());
			if(p)
				ShowGUIProperties(p);
			else
			{
				auto p = dynamic_cast<Light*>(node.get());
				if (p)
					ShowGUIProperties(p);
				else
				{
					auto p = dynamic_cast<SceneNode*>(node.get());
					if (p)
						ShowGUIProperties(p);
					else
						ShowGUIProperties(node.get());
				}
			}
		}
    }
}

void Editor::ShowHierachy()
{
    //if (ImGui::CollapsingHeader("Hierachy"))
    {
        auto scene = scene_.lock();
        if (scene)
            ShowGUIHierarchy(scene.get());
    }
}

PTexture Editor::GetMaterialPreview(PMaterial material)
{
    if (previewFrameBuffer_->IsReady())
    {
        auto graphics = RenderingContext::GetPtr();
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
        auto graphics = RenderingContext::GetPtr();
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
        auto graphics = RenderingContext::GetPtr();
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

void Editor::ShowGUIHierarchy(Node* node)
{
	if (dynamic_cast<EditorSceneNode*>(node))
		return;
	auto selectedNode = GetNode();
	auto name = node->GetName();
	if (selectedNode == node)
		name = "*" + name;
	if (GetSceneChildren(node) > 0)
	{
		if (ImGui::TreeNode(("##" + node->GetName()).c_str()))
		{
			ImGui::SameLine();
			if (ImGui::SmallButton(name.c_str()))
				SetNode(SharedFromPointerNode(node));

			auto& children = node->GetChildren();
			for (auto child : children)
				ShowGUIHierarchy(child.get());
			ImGui::TreePop();
		}
		else
		{
			ImGui::SameLine();
			if (ImGui::SmallButton(name.c_str()))
				SetNode(SharedFromPointerNode(node));
		}
	}
	else
	{
		if (ImGui::SmallButton(name.c_str()))
			SetNode(SharedFromPointerNode(node));
	}
}

int Editor::GetSceneChildren(Node* node) const
{
	int n = 0;
	auto& children = node->GetChildren();
	for (auto child : children)
	{
		if (!dynamic_cast<EditorSceneNode*>(child.get()))
			++n;
	}
	return n;
}


void Editor::ShowGUIProperties(Node* node)
{
	std::string header = "Transform:" + node->GetName();
	if (ImGui::TreeNode(header.c_str()))
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


void Editor::ShowGUIProperties(SceneNode* node)
{
	ShowGUIProperties(static_cast<Node*>(node));
	if (node->GetMaterial())
		ShowGUIProperties(node->GetMaterial().get());
}

void Editor::ShowGUIProperties(Material* material)
{
	std::string header = "Material:" + material->GetName();
	if (ImGui::TreeNode(header.c_str()))
	{
		if (ImGui::TreeNode("Preview"))
		{
			auto texture = GetMaterialPreview(SharedFromPointer(material));
			if (texture && texture->IsReady())
				ImGui::Image((void*)(intptr_t)texture->GetID(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			ImGui::TreePop();
		}

		if (material->HasTextures() && ImGui::TreeNode("Textures"))
		{
			for (int i = 0; i < (int)MaterialTexture::MAX_MAPS; i++)
			{
				auto texture = material->GetTexture((MaterialTexture)i);
				if (texture && texture->IsReady())
				{
					auto type = texture->GetMapType();
					if (ImGui::TreeNode(ToString(type)))
					{
						auto width = std::min(64.f, (float)texture->GetWidth());
						auto height = std::min(64.f, (float)texture->GetHeight());
						ImGui::Image((void*)(intptr_t)texture->GetID(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
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
		ImGui::Combo("Render Pass", (int*)&renderPass, "Vertex Color\0Unlit\0Lit\0Text\0Blend\0Blur\0Wave\0Show Diffuse\0\0");
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
		ImGui::DragFloat("##bias", &shadowBias, 1.f, 0.0f, 10.0f, "Shadow Bias %.3f");
		material->SetBias(shadowBias);

		ImGui::TreePop();
	}
}

void Editor::ShowGUIProperties(Light* light)
{
	ShowGUIProperties(static_cast<SceneNode*>(light));
	std::string header = "Light:" + light->GetName();
	if (ImGui::TreeNode(header.c_str()))
	{
		auto type = light->GetType();
		ImGui::Combo("Type", (int*)&type, "Point\0Directional\0Spot");
		light->SetType(type);

		if (type == LightType::SPOT)
		{
			auto cutOff = light->GetSpotCutOff();
			ImGui::SliderAngle("CutOff", &cutOff, 0.f, 180.f);
			light->SetSpotCutOff(cutOff);
		}

		if (type != LightType::DIRECTIONAL)
		{
			auto distance = light->GetDistance();
			ImGui::DragFloat("##distance", &distance, 1.f, 0.f, MAX_WORLD_SIZE, "Size %.1f");
			light->SetDistance(distance);
		}

		auto energy = light->GetEnergy();
		ImGui::DragFloat("##energy", &energy, 0.1f, 0.f, 1000.f, "Energy %.1f");
		light->SetEnergy(energy);

		auto color = light->GetColor();
		ImGui::ColorEdit3("Color", &color[0]);
		light->SetColor(color);

		auto enableDiffuse = light->IsDiffuseEnabled();
		ImGui::Checkbox("Diffuse", &enableDiffuse);
		light->EnableDiffuseColor(enableDiffuse);

		auto enableSpecular = light->IsSpecularEnabled();
		ImGui::Checkbox("Specular", &enableSpecular);
		light->EnableSpecularColor(enableSpecular);

		auto shadow = light->DoShadows();
		ImGui::Checkbox("Shadows", &shadow);
		light->EnableShadows(shadow);

		if (shadow)
		{
			auto bias = light->GetBias();
			ImGui::DragFloat("##bias", &bias, 1.f, 0.0f, 5.0f, "Bias %.3f");
			light->SetBias(bias);
		}
		ImGui::TreePop();
	}
}

void Editor::ShowGUIProperties(Camera* camera)
{
	ShowGUIProperties(static_cast<SceneNode*>(camera));
	std::string header = "Camera:" + camera->GetName();
	if (ImGui::TreeNode(header.c_str()))
	{
		auto zNear = camera->GetZNear();
		ImGui::DragFloat("##zNear", &zNear, 1.f, 0.1f, 10000.0f, "zNear %.1f");
		camera->SetNearClip(zNear);

		auto zFar = camera->GetZFar();
		ImGui::DragFloat("##zFar", &zFar, 1.f, 0.1f, 10000.0f, "zFar %.1f");
		camera->SetFarClip(zFar);

		auto isOrtho = camera->IsOrtho();
		ImGui::Checkbox("Ortho", &isOrtho);
		isOrtho ? camera->EnableOrtho() : camera->DisableOrtho();

		if (!isOrtho)
		{
			auto fov = camera->GetFOVRadians();
			ImGui::SliderAngle("FOV", &fov, 0.f, CAMERA_MAX_FOV_DEGREES);
			camera->SetFOVRadians(fov);
		}
		else
		{
			auto orthoScale = camera->GetOrthoScale();
			ImGui::DragFloat("##orthoScale", &orthoScale, 1.f, 0.f, MAX_WORLD_SIZE, "Size %.1f");
			camera->SetOrthoScale(orthoScale);
		}
		ImGui::TreePop();
	}
}
