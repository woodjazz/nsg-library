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
#include "Explo.h"
#include "GameObject.h"
#include "Level.h"
#include <random>
static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_real_distribution<float> dist(0.0, TWO_PI);
static const auto TEX_SIZE = 1 / 7.f;

Explo::Explo(PSceneNode node) : totalTime_(0), alpha_(1) {
    auto mesh = QuadMesh::GetNDC();
    material_ = Material::Create();
    texture_ = std::make_shared<Texture2D>(Resource::Get("data/explo.png"));
    material_->SetTexture(texture_);
    material_->SetRenderPass(RenderPass::UNLIT);
    material_->EnableTransparent(true);
    material_->SetBillboardType(
        BillboardType::SPHERICAL); // BillboardType::CYLINDRICAL_Z);
    sprite_ = node->CreateChild<SceneNode>();
    sprite_->Roll(dist(mt));
    sprite_->Hide(true);
    sprite_->SetMesh(mesh);
    sprite_->SetMaterial(material_);
    uvTransform_ = Vector4(TEX_SIZE, TEX_SIZE, 0, 0);
    texture_->SetUVTransform(uvTransform_);
    filter_ = Material::GetOrCreate("ShockWaveMaterial");
    filter_->SetRenderPass(RenderPass::SHOCKWAVE);
}

Explo::~Explo() {}

void Explo::Start() {
    if (slotUpdate_)
        return;

    sprite_->GetParent()->Hide(true, false);
    sprite_->Hide(false);

    slotUpdate_ =
        Engine::GetPtr()->SigUpdate()->Connect([this](float deltaTime) {
            auto data = filter_->GetShockWaveFilter();
            data.time_ += deltaTime * 0.3f;
            if (data.time_ > 0.5f) {
                Level::GetCurrent()->GetCamera()->RemoveFilter(filter_);
                data.time_ = 0;
            }
            filter_->SetFilterShockWave(data);

            const auto fps = 1 / 24.f;
            if (totalTime_ > fps) {
                totalTime_ = 0;
                if (uvTransform_.z >= 1) {
                    uvTransform_.z = 0;
                    uvTransform_.w += TEX_SIZE;
                    if (uvTransform_.w >= 1) {
                        uvTransform_.z = 0;
                        uvTransform_.w = 0;
                        filter_->SetFilterShockWave(ShockWaveFilter());
                        auto obj = static_cast<GameObject*>(
                            sprite_->GetParent()->GetUserData());
                        auto p = obj->shared_from_this();
                        p->Destroyed();
                        slotUpdate_ = nullptr;
                        return;
                    }
                }
                texture_->SetUVTransform(uvTransform_);
                uvTransform_.z += TEX_SIZE;
                if (uvTransform_.w >= 0.95f - TEX_SIZE)
                    alpha_ -= TEX_SIZE;
                material_->SetAlpha(alpha_);
            } else
                totalTime_ += deltaTime;
        });

    Level::GetCurrent()->GetCamera()->AddFilter(filter_);

    auto camera = sprite_->GetScene()->GetMainCamera();
    auto posNDC = camera->WorldToScreen(sprite_->GetGlobalPosition());
    auto data = filter_->GetShockWaveFilter();
    data.time_ = 0;
    data.center_ = Vector2(posNDC.x, posNDC.y) * .5f + .5f;
    filter_->SetFilterShockWave(data);
}
