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
#include "AnimationController.h"
#include "AnimationState.h"
#include "Check.h"
#include "Maths.h"
#include "Scene.h"
#include "SceneNode.h"
#include <algorithm>

namespace NSG {
AnimationControl::AnimationControl()
    : targetWeight_(1), speed_(1), fadeTime_(0) {}

AnimationController::AnimationController(PSceneNode sceneNode)
    : sceneNode_(sceneNode) {
    slotUpdate_ = sceneNode->GetScene()->SigUpdate()->Connect(
        [this](float deltaTime) { Update(deltaTime); });
}

AnimationController::~AnimationController() {}

PAnimationControl
AnimationController::GetAnimationControl(const std::string& name) {
    auto it = animations_.find(name);
    if (it == animations_.end()) {
        auto animation = Animation::Get(name);
        CHECK_CONDITION(animation->IsReady());
        auto clone = animation->Clone();
        auto sceneNode = sceneNode_.lock();
        clone->ResolveFor(sceneNode);
        PAnimationControl control = std::make_shared<AnimationControl>();
        control->animation_ = clone;
        animations_[name] = control;
        return control;
    }
    return it->second;
}

PAnimationState AnimationController::GetAnimationState(PAnimation animation) {
    auto it = animationStates_.begin();
    while (it != animationStates_.end()) {
        auto animState = *it;
        if (animation == animState->GetAnimation())
            return animState;
        ++it;
    }
    return nullptr;
}

void AnimationController::Play(const std::string& name, bool looped,
                               float fadeInTime) {
    auto control = GetAnimationControl(name);
    control->fadeTime_ = fadeInTime;
    control->targetWeight_ = 1;
    auto animationState = GetAnimationState(control->animation_);
    if (!animationState) {
        animationState = std::make_shared<AnimationState>(control->animation_);
        animationStates_.push_back(animationState);
    }
    animationState->SetLooped(looped);
}

void AnimationController::CrossFade(const std::string& name, bool looped,
                                    float fadeTime) {
    FadeOthers(name, 0, fadeTime);
    Play(name, looped, fadeTime);
}

void AnimationController::Stop(const std::string& name, float fadeOutTime) {
    auto control = GetAnimationControl(name);
    control->fadeTime_ = fadeOutTime;
    control->targetWeight_ = 0;
}

void AnimationController::Blend(const std::string& name, float targetWeight,
                                float fadeTime) {
    auto control = GetAnimationControl(name);
    control->targetWeight_ = Clamp(targetWeight, 0.f, 1.f);
    control->fadeTime_ = fadeTime;
}

bool AnimationController::IsPlaying(const std::string& name) const {
    for (auto& state : animationStates_)
        if (name == state->GetAnimation()->GetName())
            return true;
    return false;
}

void AnimationController::SetSpeed(const std::string& name, float speed) {
    auto control = GetAnimationControl(name);
    control->speed_ = speed;
}

void AnimationController::FadeOthers(const std::string& name,
                                     float targetWeight, float fadeTime) {
    auto control = GetAnimationControl(name);
    for (auto it : animations_) {
        auto other = it.second;
        if (other != control) {
            other->targetWeight_ = Clamp(targetWeight, 0.f, 1.f);
            other->fadeTime_ = fadeTime;
        }
    }
}

AnimationController::Animations::iterator
AnimationController::Remove(AnimationController::Animations::iterator it,
                            PAnimationControl control) {
    animationStates_.erase(
        std::remove_if(animationStates_.begin(), animationStates_.end(),
                       [&](PAnimationState obj) {
                           return control->animation_ == obj->GetAnimation();
                       }),
        animationStates_.end());
    return animations_.erase(it);
};

void AnimationController::Update(float deltaTime) {
    auto it = animations_.begin();
    while (it != animations_.end()) {
        auto control = it->second;
        auto state = GetAnimationState(control->animation_);
        if (!state) {
            it = Remove(it, control);
            continue;
        } else {
            if (control->speed_ != 0)
                state->AddTime(control->speed_ * deltaTime);
            auto targetWeight = control->targetWeight_;
            auto fadeTime = control->fadeTime_;
            auto currentWeight = state->GetWeight();
            if (currentWeight != targetWeight) {
                if (fadeTime > 0) // fading
                {
                    auto delta = 1.f / fadeTime * deltaTime;
                    if (currentWeight < targetWeight)
                        currentWeight =
                            std::min(currentWeight + delta, targetWeight);
                    else if (currentWeight > targetWeight)
                        currentWeight =
                            std::max(currentWeight - delta, targetWeight);
                    state->SetWeight(currentWeight);
                } else
                    state->SetWeight(targetWeight);
            }

            if (state->GetWeight() == 0 &&
                (targetWeight == 0 || fadeTime == 0)) {
                it = Remove(it, control);
                continue;
            }
        }
        ++it;
    }

    for (auto& state : animationStates_)
        state->Update();
}
}
