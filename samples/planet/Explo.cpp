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
#include "Explo.h"
#include "GameObject.h"
#include <random>
static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_real_distribution<float> dist(0.0, TWO_PI);
Explo::Explo(PSceneNode node)
	: totalTime_(0),
	alpha_(1),
    start_(false)
{
    auto mesh = Mesh::GetOrCreate<QuadMesh>("QuadMesh");
	material_ = Material::Create();
	texture_ = std::make_shared<Texture2D>(Resource::GetOrCreate<ResourceFile>("data/explo.png"));
    material_->SetTexture(texture_);
    material_->SetRenderPass(RenderPass::UNLIT);
	material_->EnableTransparent(true);
    material_->SetBillboardType(BillboardType::CYLINDRICAL_Z);
	sprite_ = node->CreateChild<SceneNode>();
	sprite_->Roll(dist(mt));
    sprite_->Hide(true);
	sprite_->SetMesh(mesh);
	sprite_->SetMaterial(material_);
	static const auto texSize = 1 / 7.f;
    uvTransform_ = Vector4(texSize, texSize, 0, 0);
	texture_->SetUVTransform(uvTransform_);
    slotUpdate_ = Engine::GetPtr()->SigUpdate()->Connect([this](float deltaTime)
    {
        if(!start_)
            return;

		const auto fps = 1 / 24.f;
        if (totalTime_ > fps)
        {
            totalTime_ = 0;
            if (uvTransform_.z >= 1)
            {
                uvTransform_.z = 0;
                uvTransform_.w += texSize;
                if (uvTransform_.w >= 1)
                {
					auto obj = static_cast<GameObject*>(sprite_->GetParent()->GetUserData());
					auto p = obj->shared_from_this();
					if (p.use_count() > 1)
					{
						p->SigDestroyed()->Run();
						if (p.use_count() > 1)
							GameObject::SigOneDestroyed()->Run(obj);
					}
					return;
                }
            }
			texture_->SetUVTransform(uvTransform_);
            uvTransform_.z += texSize;
            if (uvTransform_.w >= 0.95f-texSize)
                alpha_ -= texSize;
            material_->SetAlpha(alpha_);
        }
        else
            totalTime_ += deltaTime;
    });
	sound_ = Sound::Create();
	sound_->Set(Resource::GetOrCreate<ResourceFile>("data/explo.wav"));
	sound_->TryReady();
}

Explo::~Explo()
{

}

void Explo::Fire()
{
    sprite_->GetParent()->Hide(true, false);
    sprite_->Hide(false);
    start_ = true;
    sound_->Play();
}