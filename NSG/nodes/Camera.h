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
#pragma once
#include "Types.h"
#include "Util.h"
#include "SceneNode.h"

namespace NSG
{
    class Camera : public SceneNode
    {
    public:
        Camera(const std::string& name = GetUniqueName("Camera"));
        ~Camera();
        void SetWindow(Window* window);
        void EnableOrtho();
        void DisableOrtho();
        void SetFOV(float fovy); // in degrees
        void SetHalfHorizontalFov(float hhfov); // in radians
        void SetNearClip(float zNear);
        void SetFarClip(float zFar);
        void EnableAutoAspectRatio(bool enable) { autoAspectRatio_ = enable; }
        void SetAspectRatio(unsigned width, unsigned height);
        void SetAspectRatio(float aspect);
        float GetAspectRatio() const { return aspectRatio_; }
        void SetViewportFactor(const Vector4& viewportFactor);
        const Vector4& GetViewportFactor() const { return viewportFactor_; }
        void SetOrthoScale(float orthoScale);
        float GetOrthoScale() const { return orthoScale_; }
        bool IsOrtho() const { return isOrtho_; }
        void SetSensorFit(CameraSensorFit sensorFit);
        //XYZ are in normalized device coordinates (-1, 1)
        Vertex3 ScreenToWorld(const Vertex3& screenXYZ) const;
        //Returned XYZ values are in normalized device coordinates (-1, 1)
        //Returned Z value is interpreted as a distance from the camera.
        Vertex3 WorldToScreen(const Vertex3& worldXYZ) const;
        //XY are in normalized device coordinates (-1, 1)
        Ray GetScreenRay(float screenX, float screenY) const;
        static Ray GetRay(float screenX, float screenY);
        const Matrix4& GetView() const;
        const Matrix4& GetViewProjection() const;
        const Matrix4& GetProjection() const;
        const PFrustum GetFrustum() const;
        const Frustum* GetFrustumPointer() const;
        PFrustum GetFrustumSplit(float nearSplit, float farSplit) const;
        bool IsVisible(const Node& node, Mesh& mesh) const;
        bool IsVisible(const SceneNode& node) const;
        bool IsVisible(const BoundingBox& bb) const;
        void OnDirty() const override;
        float GetZNear() const { return zNear_; }
        float GetZFar() const { return zFar_; }
        float GetFov() const { return fovy_; }
        void Save(pugi::xml_node& node) const override;
        void Load(const pugi::xml_node& node) override;
        const OrthoProjection& GetOrthoProjection() const;
        void UnRegisterWindow();
        void EnableAutomaticSplits(bool enable) { automaticSplits_ = enable; }
        bool AutomaticSplits() const { return automaticSplits_; }
        void SetMaxShadowSplits(int splits);
        int GetMaxShadowSplits() const;
        void EnableColorSplits(bool enable);
        void FillShaderDefines(std::string& defines, PassType passType) const;
        // This is a logarithmic factor to make the (shadow) splits.
        // For 4 splits: 
        // factor=0 => 25% 50% 75% 100%
        // factor=0.1 => 22% 45% 69% 100%
        // factor=1 => 0.5% 3.1% 17% 100%
        void SetShadowSplitLogFactor(float factor); // between 0-1
        float GetShadowSplitLogFactor() const;
        static BoundingBox GetViewBox(const Frustum* frustum, const Scene* scene, bool receivers, bool casters);
        static SignalCamera::PSignal SignalBeingDestroy();
		void SetOrthoProjection(OrthoProjection projection);
		void DisableUserOrthoProjection() { hasUserOrthoProjection_ = false; }
        void Debug(DebugRenderer* debugRenderer, const Color& color);
    private:
    	const Matrix4& GetViewProjectionInverse() const;
        OrthoProjection CalculateOrthoProjection(float zNear, float zFar) const;
        float CalculateAspectRatio() const;
        void SetScale(const Vertex3& scale); // not implemented (does not make sense for cameras and will make normals wrong)
        void UpdateProjection() const;
        void UpdateViewProjection() const;
        void UpdateFrustum();
        void Update() const;
        mutable Matrix4 matView_;
        mutable Matrix4 matViewInverse_;
        mutable Matrix4 matProjection_;
        mutable Matrix4 matViewProjection_;
        mutable Matrix4 matViewProjectionInverse_;
        float fovy_; // in radians
        float zNear_;
        float zFar_;
        Vector4 viewportFactor_;
        bool isOrtho_;
        unsigned viewWidth_;
        unsigned viewHeight_;
        float aspectRatio_;
        mutable PFrustum frustum_;
        SignalSizeChanged::PSlot slotViewChanged_;
        Window* window_;
        SignalWindow::PSlot slotWindow_;
        float orthoScale_;
        CameraSensorFit sensorFit_;
        mutable OrthoProjection orthoProjection_;
        mutable bool isDirty_;
        bool autoAspectRatio_;
        int shadowSplits_;
        bool colorSplits_; // used to debug splits in shader (each split in one color)
        float shadowSplitLogFactor_;
        bool automaticSplits_;
		bool hasUserOrthoProjection_;
    };
}
