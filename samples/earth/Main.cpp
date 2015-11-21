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

#include "NSG.h"
#include <deque>
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

	auto window = Window::Create();
    std::deque<Vertex3> camControlPoints;
    auto scene = std::make_shared<Scene>();
    scene->SetAmbientColor(ColorRGB(0.1f));

    auto camera = scene->CreateChild<Camera>();
	{
        camControlPoints.push_back(Vertex3(-10.0f, 0.0f, 0.0f));
        camControlPoints.push_back(Vertex3(0.0f, 0.0f, 10.0f));
        camControlPoints.push_back(Vertex3(10.0f, 0.0f, 0.0f));
        camControlPoints.push_back(Vertex3(0.0f, 0.0f, -10.0f));
        camera->SetPosition(Vertex3(0, 0, 10));
        camera->SetGlobalLookAtPosition(Vertex3(0));
    }

	auto sphereMesh(Mesh::CreateClass<SphereMesh>());
	sphereMesh->Set(1.5f, 24);
    auto earth = scene->CreateChild<SceneNode>();
    {
        earth->SetMesh(sphereMesh);
		auto resource = Resource::GetOrCreateClass<ResourceFile>("data/Earthmap720x360_grid.jpg");
        auto texture = std::make_shared<Texture2D>(resource, (int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
		auto material(Material::Create());
		material->SetRenderPass(RenderPass::LIT);
		material->SetTexture(texture);
        material->SetDiffuseColor(ColorRGB(0.8f, 0.8f, 0.8f));
		material->SetSpecularColor(ColorRGB(1.0f, 0.0f, 0.0f));
        material->SetShininess(10);
        earth->SetMaterial(material);
        earth->SetPosition(Vertex3(5, 0, 0));
    }

	auto metalPlanet = scene->CreateChild<SceneNode>();
	{
		metalPlanet->SetMesh(sphereMesh);
		auto resource0 = Resource::GetOrCreateClass<ResourceFile>("data/wall_COLOR.png");
		auto texture0 = std::make_shared<Texture2D>(resource0, (int)TextureFlag::GENERATE_MIPMAPS);
		auto resource1 = Resource::GetOrCreateClass<ResourceFile>("data/wall_NRM.png");
		auto texture1 = std::make_shared<Texture2D>(resource1, (int)TextureFlag::GENERATE_MIPMAPS);
		texture1->SetMapType(TextureType::NORM);
		texture0->SetFilterMode(TextureFilterMode::NEAREST);
		texture1->SetFilterMode(TextureFilterMode::NEAREST);
		auto material(Material::Create());
		material->SetRenderPass(RenderPass::LIT);
		material->SetTexture(texture0);
		material->SetTexture(texture1);
		material->SetDiffuseColor(ColorRGB(0.1f, 0.4f, 0.8f));
		material->SetShininess(0.7f);
		metalPlanet->SetMaterial(material);
		metalPlanet->SetPosition(Vertex3(-5, 0, 0));
	}

	auto transparentPlanet = scene->CreateChild<SceneNode>();
	{
		transparentPlanet->SetMesh(sphereMesh);
		auto resource0 = Resource::GetOrCreateClass<ResourceFile>("data/decalspatches.png");
		auto texture0 = std::make_shared<Texture2D>(resource0, (int)TextureFlag::GENERATE_MIPMAPS);
		texture0->SetUseAlpha(true);
		auto material(Material::Create());
		material->SetRenderPass(RenderPass::LIT);
		material->SetDiffuseColor(ColorRGB(0.f, 0.f, 1.f));
		material->SetAlpha(0);
		material->EnableTransparent(true);
		material->SetTexture(texture0);
		transparentPlanet->SetMaterial(material);
		transparentPlanet->SetPosition(Vertex3(0, 0, -5));
	}

    auto light = scene->CreateChild<Light>();
    light->EnableShadows(false);
    {
		auto material(Material::Create());
		material->SetRenderPass(RenderPass::LIT);
		material->SetDiffuseColor(ColorRGB(1, 0, 0));
        light->SetMaterial(material);
		auto mesh(Mesh::CreateClass<SphereMesh>());
		mesh->Set(0.2f, 16);
        light->SetMesh(mesh);
        light->SetPosition(Vertex3(-10, 0, 5));
    }

	auto resource = Resource::GetOrCreate<ResourceFile>("data/nice_music.ogg");
    auto music = Music::Create();
	music->Set(resource);
    music->Play();

	auto slotUpdate = Engine::SigUpdate()->Connect([&](float deltaTime)
    {
        {
            static float delta1 = 0;
			auto position = CatmullRom(
                                   camControlPoints[0],
                                   camControlPoints[1],
                                   camControlPoints[2],
                                   camControlPoints[3],
                                   delta1);

            camera->SetPosition(position);
            camera->SetGlobalLookAtPosition(VECTOR3_ZERO);
            delta1 += deltaTime * 0.1f;
            if (delta1 > 1)
            {
                delta1 = 0;
                auto p = camControlPoints.front();
                camControlPoints.pop_front();
                camControlPoints.push_back(p);
            }
        }

		auto angle = PI10 * deltaTime;
		earth->Roll(angle);
		earth->Yaw(angle);
		metalPlanet->Yaw(angle);
		metalPlanet->Pitch(angle);
		transparentPlanet->Roll(angle);

		{
			static auto factor = 1.f;
			static auto delta = 0.f;
			metalPlanet->SetScale(1 + delta);
			delta += factor * deltaTime;
			if (delta > 1 || delta < 0)
				factor *= -1;
		}
    });

	window->SetScene(scene.get());
    return Engine::Create()->Run();
}

