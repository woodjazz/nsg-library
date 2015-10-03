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
#include "EditorCamera.h"
#include "EditorFrustum.h"
#include "QuadMesh.h"
#include "Material.h"
#include "Camera.h"

namespace NSG
{
    EditorCamera::EditorCamera(const std::string& name)
        : EditorSceneNode(name)
    {
		SetMesh(Mesh::Create<QuadMesh>());
        SetMaterial(Material::GetOrCreate("NSGEditorCamera"));
        material_->SetRenderPass(RenderPass::UNLIT);
        material_->SetDiffuseColor(COLOR_RED);
		material_->EnableTransparent(true);
		material_->SetAlpha(0.25f);
		material_->SetBillboardType(BillboardType::SPHERICAL);
    }

    EditorCamera::~EditorCamera()
    {

    }

    void EditorCamera::SetCamera(PCamera camera)
    {
        if(!frustum_)
			frustum_ = camera->CreateChild<EditorFrustum>(GetUniqueName("EditorFrustum"));
		frustum_->SetCamera(camera);
    }
}
