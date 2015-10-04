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
#include "EditorLight.h"
#include "EditorFrustum.h"
#include "CircleMesh.h"
#include "Material.h"
#include "Camera.h"

namespace NSG
{
    EditorLight::EditorLight(const std::string& name)
        : EditorSceneNode(name)
    {
        SetMesh(Mesh::Create<CircleMesh>());
        SetMaterial(Material::GetOrCreate("NSGEditorLight"));
        material_->SetRenderPass(RenderPass::UNLIT);
        material_->SetDiffuseColor(COLOR_YELLOW);
        material_->EnableTransparent(true);
        material_->SetAlpha(0.25f);
        material_->SetBillboardType(BillboardType::SPHERICAL);
        material_->CastShadow(false);
        material_->ReceiveShadows(false);

    }

    EditorLight::~EditorLight()
    {

    }
}
