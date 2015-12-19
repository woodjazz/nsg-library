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
#include "QuadMesh.h"
#include "SphereMesh.h"
#include "Material.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Light.h"

#define X (unsigned char)0xFF,
#define O (unsigned char)0x00,

EditorLight::EditorLight(const std::string& name)
    : EditorSceneNode(name)
{
    SetMesh(Mesh::GetOrCreate<QuadMesh>("NSGEditorCamera"));
    SetMaterial(Material::GetOrCreate("NSGEditorLight"));
    material_->SetRenderPass(RenderPass::UNLIT);
    material_->SetDiffuseColor(COLOR_YELLOW);
    material_->EnableTransparent(true);
	material_->SetAlpha(0.9f);
    material_->SetBillboardType(BillboardType::SPHERICAL);
    material_->CastShadow(false);
    material_->ReceiveShadows(false);

    const int SIZE = 8;
    static const unsigned char image[SIZE][SIZE]
    {
        {X X X X X X X X},
        {X O O O O O O X},
        {X O X O O O O X},
        {X O X O O O O X},
        {X O X O O O O X},
        {X O X X X X O X},
        {X O O O O O O X},
        {X X X X X X X X},
    };

    auto texture = std::make_shared<Texture2D>();
    texture->SetFormat(GL_ALPHA);
    texture->SetData(&image[0][0]);
    texture->SetSize(SIZE, SIZE);
    texture->SetMapType(TextureType::COL);
	texture->SetFilterMode(TextureFilterMode::NEAREST);
    material_->SetTextMap(texture);
}

EditorLight::~EditorLight()
{
}

void EditorLight::OnCreated()
{
    child_ = CreateChild<EditorSceneNode>(GetUniqueName("EditorLight"));
	auto light = std::dynamic_pointer_cast<Light>(GetParent());
	ConfigureChild(light.get(), child_.get());
    slotSetType_ = light->SignalSetType()->Connect([this](Light* light)
    {
        ConfigureChild(light, child_.get());
    });
}

void EditorLight::ConfigureChild(const Light* light, SceneNode* child)
{
	auto distance = light->GetDistance();
	auto mesh = Mesh::GetOrCreate<SphereMesh>(GetName() + "SphereMesh");
	mesh->Set(distance);
	child->SetMesh(mesh);
	child->SetMaterial(material_);
}

