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
#include "Planet.h"

Planet::Planet(PScene scene)
    : planet_(scene->CreateChild<SceneNode>()),
      clouds_(planet_->CreateChild<SceneNode>()),
      grid_(planet_->CreateChild<SceneNode>("Grid"))
{
	auto colorTexture = std::make_shared<Texture2D>(
		ResourceFile::GetOrCreate("data/earthcolor.jpg"),
		(int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y
		);

	auto specularTexture = std::make_shared<Texture2D>(
		ResourceFile::GetOrCreate("data/earthspecular.jpg"),
		(int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y
		);

	specularTexture->SetMapType(TextureType::SPEC);

	auto normalTexture = std::make_shared<Texture2D>(
		Resource::Get("data/earthnormal.jpg"),
		(int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y
		);

	normalTexture->SetMapType(TextureType::NORM);

	auto cloudsTexture = std::make_shared<Texture2D>(
		Resource::Get("data/clouds.jpg"),
		(int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y
		);

    planet_->SetMesh(Planet::GetMesh());
	planet_->SetScale(2.5f);
    {
        auto material(Material::Create());
        material->SetRenderPass(RenderPass::LIT);
        material->SetTexture(colorTexture);
        material->SetTexture(specularTexture);
        material->SetTexture(normalTexture);
        material->SetDiffuseColor(ColorRGB(0.8f, 0.8f, 0.8f));
        //material->SetSpecularColor(ColorRGB(1.0f, 0.0f, 0.0f));
        material->SetAmbientIntensity(3.f);
        material->SetShininess(10);
        planet_->SetMaterial(material);
    }

    clouds_->SetMesh(Planet::GetMesh());
	clouds_->SetScale(1.15f);
    {
        auto material(Material::Create());
        material->SetRenderPass(RenderPass::LIT);
        material->SetTexture(cloudsTexture);
        material->SetDiffuseColor(COLOR_SKY_BLUE);
        material->SetSpecularIntensity(0);
        material->EnableTransparent(true);
        material->SetAlpha(0.15f);
        material->SetEmitIntensity(0.75f);
        clouds_->SetMaterial(material);
    }

    grid_->SetMesh(Planet::GetGridMesh());
	grid_->SetScale(1.3f);
    {
        auto material(Material::Create());
        material->SetFillMode(FillMode::WIREFRAME);
        material->SetRenderPass(RenderPass::LIT);
        material->SetEmitIntensity(25);
        material->SetDiffuseColor(COLOR_PEACH_PUFF);
        material->EnableTransparent(true);
        material->SetAlpha(0.03f);
        grid_->SetMaterial(material);
    }

	updateSlot_ = Engine::SigUpdate()->Connect([&](float deltaTime)
    {
        auto angle = PI10 * deltaTime;
        clouds_->Roll(angle * 0.5f);
    });
}

Planet::~Planet()
{
}

PSphereMesh Planet::GetMesh()
{
    auto mesh(Mesh::GetOrCreateClass<SphereMesh>("PlanetMesh"));
    mesh->Set(1.f, 36);
    return mesh;
}

PIcoSphereMesh Planet::GetGridMesh()
{
    auto mesh(Mesh::GetOrCreateClass<IcoSphereMesh>("GridMesh"));
    mesh->Set(1.f, 4);
    return mesh;
}