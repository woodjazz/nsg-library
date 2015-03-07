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
#include "Camera.h"
#include "App.h"
#include "Frustum.h"
#include "Graphics.h"
#include "Util.h"
#include "Ray.h"
#include "FilterBlur.h"
#include "FilterBlend.h"
#include "Program.h"
#include "Scene.h"
#include "ShowTexture.h"
#include "Render2Texture.h"
#include "Window.h"
#include "SignalSlots.h"
#include "pugixml.hpp"
#include <sstream>

namespace NSG
{
    Camera::Camera(const std::string& name)
        : SceneNode(name),
          fovy_(glm::radians(45.0f)),
          zNear_(0.1f),
          zFar_(250),
          xo_(0),
          yo_(0),
          xf_(1),
          yf_(1),
          isOrtho_(false),
          orthoCoords_(-1.0f, 1.0f, -1.0f, 1.0f),
          graphics_(Graphics::this_),
          viewWidth_(0),
          viewHeight_(0),
          aspectRatio_(1),
        cameraIsDirty_(false)
    {
        SetInheritScale(false);
        UpdateProjection();
        graphics_->SetCamera(this);
        auto window = graphics_->GetWindow();
        if(window)
            SetWindow(window);
    }

    Camera::~Camera()
    {
        if(graphics_->GetCamera() == this)
            graphics_->SetCamera(nullptr);
    }

    void Camera::SetWindow(Window* window)
    {
        if(window)
        {
            SetAspectRatio(window->GetWidth(), window->GetHeight());

            slotViewChanged_ = window->signalViewChanged_->Connect([&](int width, int height)
            {
                SetAspectRatio(width, height);
            });
        }
        else
        {
            slotViewChanged_ = nullptr;
            SetAspectRatio(1);
        }
    }

	void Camera::SetAspectRatio(unsigned width, unsigned height)
    {
        if (viewWidth_ != width || viewHeight_ != height)
        {
            viewWidth_ = width;
            viewHeight_ = height;
            float aspect = 1;
            if (height > 0)
                aspect = static_cast<float>(width) / height;
            SetAspectRatio(aspect);

			if (render2Texture_)
				render2Texture_->SetSize(width, height);
        }
    }

    void Camera::SetAspectRatio(float aspect)
    {
        CHECK_ASSERT(aspect != 0, __FILE__, __LINE__);
        if(aspectRatio_ != aspect)
        {
            aspectRatio_ = aspect;
            UpdateProjection();

            if (render2Texture_)
            {
                auto newRender2Texture = std::make_shared<Render2Texture>(GetUniqueName(name_), viewWidth_, viewHeight_);
                for (auto& filter : filters_)
                    if (filter->GetInputTexture() == render2Texture_->GetTexture())
                        filter->SetInputTexture(newRender2Texture->GetTexture());
                SetRender2Texture(newRender2Texture);
            }

            if (graphics_->GetCamera() == this)
                graphics_->SetViewport(GetViewport(), false);
        }
    }

    void Camera::EnableOrtho()
    {
        if (!isOrtho_)
        {
            isOrtho_ = true;
            UpdateProjection();
        }
    }

    void Camera::DisableOrtho()
    {
        if (isOrtho_)
        {
            isOrtho_ = false;
            UpdateProjection();
        }
    }

    void Camera::SetFOV(float fovy)
    {
        fovy = glm::radians(fovy);
        if (fovy_ != fovy)
        {
            fovy_ = fovy;
            UpdateProjection();
        }
    }

    void Camera::SetHalfHorizontalFov(float hhfov)
    {
        CHECK_ASSERT(hhfov != 0, __FILE__, __LINE__);

        float fovy = hhfov / aspectRatio_;

        if (fovy_ != fovy)
        {
            fovy_ = fovy;
            UpdateProjection();
        }
    }

    void Camera::SetNearClip(float zNear)
    {
        if (zNear_ != zNear)
        {
            zNear_ = zNear;
            UpdateProjection();
        }
    }

    void Camera::SetFarClip(float zFar)
    {
        if (zFar_ != zFar)
        {
            zFar_ = zFar;
            UpdateProjection();
        }
    }

    void Camera::SetOrtho(float left, float right, float bottom, float top)
    {
        if (orthoCoords_.x != left || orthoCoords_.y != right || orthoCoords_.z != bottom || orthoCoords_.w != top)
        {
            orthoCoords_ = Vector4(left, right, bottom, top);
            UpdateProjection();
        }
    }

    void Camera::SetViewportFactor(float xo, float yo, float xf, float yf)
    {
        xo_ = xo;
        yo_ = yo;
        xf_ = xf;
        yf_ = yf;
    }

    Recti Camera::GetViewport() const
    {
        return Recti((GLsizei)(viewWidth_ * xo_), (GLsizei)(viewHeight_ * yo_), (GLsizei)(viewWidth_ * xf_), (GLsizei)(viewHeight_ * yf_));
    }

    void Camera::UpdateProjection() const
    {
        if (isOrtho_)
        {
			matProjection_ = glm::ortho(orthoCoords_.x, orthoCoords_.y, orthoCoords_.z, orthoCoords_.w, 0.0f, zFar_);
        }
        else
        {
            CHECK_ASSERT(zNear_ > 0, __FILE__, __LINE__);

            matProjection_ = glm::perspective(fovy_, aspectRatio_, zNear_, zFar_);
        }

        UpdateViewProjection();

        SetUniformsNeedUpdate();
    }

    void Camera::UpdateViewProjection() const
    {
        cameraIsDirty_ = false;

        matViewInverse_ = GetGlobalModelMatrix();
        matView_ = glm::inverse(matViewInverse_);

        matViewProjection_ = matProjection_ * matView_;
        matViewProjectionInverse_ = glm::inverse(matViewProjection_);

        frustum_ = PFrustum(new Frustum(this));
    }

    const PFrustum Camera::GetFrustum() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return frustum_;
    }

    const Frustum* Camera::GetFrustumPointer() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return frustum_.get();

    }

    const Matrix4& Camera::GetMatViewProjection() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matViewProjection_;
    }


    const Matrix4& Camera::GetMatViewProj()
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetMatViewProjection();
        }
        else
        {
            return IDENTITY_MATRIX;
        }
    }

    Matrix4 Camera::GetModelViewProjection(const Node* pNode)
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetMatViewProjection() * pNode->GetGlobalModelMatrix();
        }
        else
        {
            // if no Camera then position is in screen coordinates
            return pNode->GetGlobalModelMatrix();
        }
    }

    const Matrix4& Camera::GetMatProjection() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matProjection_;
    }

    Matrix4 Camera::GetInverseView()
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetInverseViewMatrix();
        }
        else
        {
            return IDENTITY_MATRIX;
        }
    }

    const Matrix4& Camera::GetViewMatrix()
    {
        if (Graphics::this_->GetCamera())
        {
            return Graphics::this_->GetCamera()->GetView();
        }
        else
        {
            return IDENTITY_MATRIX;
        }

    }

	const Matrix4& Camera::GetProjectionMatrix()
	{
		if (Graphics::this_->GetCamera())
		{
			return Graphics::this_->GetCamera()->GetMatProjection();
		}
		else
		{
			return IDENTITY_MATRIX;
		}
	}

    const Matrix4& Camera::GetView() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matView_;
    }

    const Matrix4& Camera::GetInverseViewMatrix() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matViewInverse_;
    }

    const Matrix4& Camera::GetViewProjectionMatrix() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matViewProjection_;
    }

    const Matrix4& Camera::GetViewProjectionInverseMatrix() const
    {
        if (cameraIsDirty_)
            UpdateProjection();

        return matViewProjectionInverse_;
    }

    Vertex3 Camera::ScreenToWorld(const Vertex3& screenXYZ) const
    {
        Vertex4 worldCoord = GetViewProjectionInverseMatrix() * Vertex4(screenXYZ, 1);
        return Vertex3(worldCoord.x / worldCoord.w, worldCoord.y / worldCoord.w, worldCoord.z / worldCoord.w);
    }

    Vertex3 Camera::WorldToScreen(const Vertex3& worldXYZ) const
    {
        Vertex4 screenCoord = GetViewProjectionMatrix() * Vertex4(worldXYZ, 1);
        return Vertex3(screenCoord.x / screenCoord.w, screenCoord.y / screenCoord.w, screenCoord.z / screenCoord.w);
    }

    Ray Camera::GetScreenRay(float screenX, float screenY) const
    {
        Vertex3 nearPoint(screenX, screenY, -1); //in normalized device coordinates (Z goes from near = -1 to far = 1)
        Vertex3 farPoint(screenX, screenY, 0); //in normalized device coordinates

        Vertex3 nearWorldCoord = ScreenToWorld(nearPoint);
        Vertex3 farWorldCoord = ScreenToWorld(farPoint);

        Vector3 direction(farWorldCoord - nearWorldCoord);

        return Ray(nearWorldCoord, direction);
    }

	Ray Camera::GetRay(float screenX, float screenY)
	{
		if (Graphics::this_->GetCamera())
			return Graphics::this_->GetCamera()->GetScreenRay(screenX, screenY);
		else
			return Ray(Vector3(screenX, screenY, 0), VECTOR3_FORWARD);
	}

    bool Camera::IsVisible(const Node& node, Mesh& mesh) const
    {
        return GetFrustum()->IsVisible(node, mesh);
    }

    void Camera::OnDirty() const
    {
        cameraIsDirty_ = true;
    }

    void Camera::Save(pugi::xml_node& node) const
    {
        if (!IsSerializable())
            return;

        {
            std::stringstream ss;
            ss << GetName();
            node.append_attribute("name") = ss.str().c_str();
        }

        node.append_attribute("nodeType") = "Camera";

        {
            std::stringstream ss;
            ss << fovy_;
            node.append_attribute("fovy") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << zNear_;
            node.append_attribute("zNear") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << zFar_;
            node.append_attribute("zFar") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << xo_;
            node.append_attribute("xo") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << yo_;
            node.append_attribute("yo") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << xf_;
            node.append_attribute("xf") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << yf_;
            node.append_attribute("yf") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << isOrtho_;
            node.append_attribute("isOrtho") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetPosition();
            node.append_attribute("position") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetOrientation();
            node.append_attribute("orientation") = ss.str().c_str();
        }

        SaveChildren(node);
    }

    void Camera::Load(const pugi::xml_node& node)
    {
        name_ = node.attribute("name").as_string();

        Vertex3 position = GetVertex3(node.attribute("position").as_string());
        SetPosition(position);

        fovy_ = node.attribute("fovy").as_float();
        zNear_ = node.attribute("zNear").as_float();
        zFar_ = node.attribute("zFar").as_float();
        xo_ = node.attribute("xo").as_float();
        yo_ = node.attribute("yo").as_float();
        xf_ = node.attribute("xf").as_float();
        yf_ = node.attribute("yf").as_float();
        isOrtho_ = node.attribute("isOrtho").as_bool();

        Quaternion orientation = GetQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);
        LoadChildren(node);
    }

    void Camera::AddBlurFilter(int output_width, int output_height)
    {
        if (!render2Texture_)
			SetRender2Texture(std::make_shared<Render2Texture>(GetUniqueName(name_), output_width, output_height));

        PFilterBlur blur;
        if (filters_.empty())
            blur = PFilterBlur(new FilterBlur(render2Texture_->GetTexture(), output_width, output_height));
        else
            blur = PFilterBlur(new FilterBlur(filters_.back()->GetTexture(), output_width, output_height));

        AddFilter(blur);
    }

    void Camera::AddBlendFilter(int output_width, int output_height)
    {
        if (!render2Texture_)
			SetRender2Texture(std::make_shared<Render2Texture>(GetUniqueName(name_), output_width, output_height, UseBuffer::DEPTH));

        CHECK_ASSERT(filters_.size() > 0, __FILE__, __LINE__);
        PFilterBlend blend;

        size_t n = filters_.size();

        if (n > 1)
            blend = PFilterBlend(new FilterBlend(filters_[n - 2]->GetTexture(), filters_[n - 1]->GetTexture(), output_width, output_height));
        else
            blend = PFilterBlend(new FilterBlend(render2Texture_->GetTexture(), filters_[0]->GetTexture(), output_width, output_height));

        AddFilter(blend);
    }

    PFilter Camera::AddUserFilter(PResource fragmentShader, int output_width, int output_height)
    {
        if (!render2Texture_)
			SetRender2Texture(std::make_shared<Render2Texture>(GetUniqueName(name_), output_width, output_height));

        PFilter filter;

        if (filters_.empty())
            filter = PFilter(new Filter("UserFilter", render2Texture_->GetTexture(), output_width, output_height));
        else
            filter = PFilter(new Filter("UserFilter", filters_.back()->GetTexture(), output_width, output_height));

        filter->GetProgram()->SetFragmentShader(fragmentShader);

        AddFilter(filter);

        return filter;
    }

    void Camera::AddFilter(PFilter filter)
    {
        filters_.push_back(filter);
        showTexture_->SetNormal(filter->GetTexture());
    }

    void Camera::Render()
    {
        if (render2Texture_)
        {
            Render(render2Texture_);
            showTexture_->Show();
        }
        else
        {
			graphics_->RenderVisibleSceneNodes();
        }
    }

    void Camera::Render(PRender2Texture render2Texture)
    {
        render2Texture->Begin();
		graphics_->RenderVisibleSceneNodes();
        render2Texture->End();
        for (auto& filter : filters_)
            filter->Draw();
    }

    void Camera::SetRender2Texture(PRender2Texture render2Texture)
    {
        if (render2Texture)
        {
            render2Texture_ = render2Texture;
            showTexture_ = PShowTexture(new ShowTexture);
            if (filters_.empty())
                showTexture_->SetNormal(render2Texture_->GetTexture());
            else
                showTexture_->SetNormal(filters_.back()->GetTexture());
        }
        else
        {
            showTexture_ = nullptr;
        }
    }

	void Camera::Render(PScene scene)
	{
		auto camera = Graphics::this_->GetCamera();
		if (camera)
			scene->Render(camera);
		else
			scene->SceneNode::DrawWithChildren();
	}
}