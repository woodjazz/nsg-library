/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "NSG.h"

#define X (unsigned char)0xFF,
#define O (unsigned char)0x00,
EditorCamera::EditorCamera(const std::string& name)
    : EditorSceneNode(name)
{
    SetMesh(Mesh::GetOrCreate<QuadMesh>("NSGEditorCamera"));
    SetMaterial(Material::GetOrCreate("NSGEditorCamera"));
    material_->SetRenderPass(RenderPass::TEXT);
    material_->SetDiffuseColor(Color::DodgerBlue);
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
        {X O X X X X O X},
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
	material_->SetTextMap(texture);
	texture->SetFilterMode(TextureFilterMode::NEAREST);
}

EditorCamera::~EditorCamera()
{

}

void EditorCamera::OnCreated()
{
    auto camera = std::dynamic_pointer_cast<Camera>(GetParent());
    if (!frustum_)
        frustum_ = camera->CreateChild<EditorFrustum>(GetUniqueName("EditorFrustum"));
    frustum_->SetCamera(camera);
}
