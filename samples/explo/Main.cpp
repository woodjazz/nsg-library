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
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

    auto window = Window::Create();
    auto resource = Resource::GetOrCreate<ResourceFile>("data/explo.jpg");
    auto scene = std::make_shared<Scene>();
    auto camera = scene->CreateChild<Camera>();
    auto control = std::make_shared<CameraControl>(camera);
    auto mesh = Mesh::Create<QuadMesh>();
    auto material = std::make_shared<Material>();
    material->SetDiffuseMap(std::make_shared<Texture>(resource));
    auto pass = material->GetTechnique()->GetPass(0);
    pass->SetBlendMode(BLEND_MODE::BLEND_ALPHA);
    auto program = pass->GetProgram();
    program->EnableFlags((int)ProgramFlag::UNLIT | (int)ProgramFlag::SPHERICAL_BILLBOARD);
    auto sprite = scene->CreateChild<SceneNode>();
	sprite->SetMesh(mesh);
	sprite->SetMaterial(material);
	Vector4 uOffset(0.25f, 0, 0, 0);
	Vector4 vOffset(0, 0.25f, 0, 0);

	auto slotUpdate = window->signalUpdate_->Connect([&](float deltaTime)
	{
		//uOffset.w += deltaTime;
		material->SetUVTransform(uOffset, vOffset);
		
	});

    auto renderSlot = window->signalRender_->Connect([&]()
    {
        scene->Render();
    });

    return Window::RunApp();
}
