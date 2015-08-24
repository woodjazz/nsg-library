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
#pragma once
#include "Types.h"
#include <vector>
#include <map>

namespace NSG
{
    struct AnimationControl
    {
        PAnimation animation_;
        float targetWeight_;
        float speed_;
        float fadeTime_;
        AnimationControl();
    };

    class AnimationController
    {
    public:
        AnimationController(PSceneNode sceneNode);
        ~AnimationController();
        PAnimationState Play(const std::string& name, bool looped, float fadeInTime = 0);
        PAnimationState PlayCrossFade(const std::string& name, bool looped, float fadeTime = 0);
        void StopAnimation(const std::string& name);
        bool IsPlaying(const std::string& name) const;
    private:
        void FadeOthers(const std::string& name, float targetWeight, float fadeTime);
        void Update(float deltaTime);
        PAnimationControl GetAnimationControl(const std::string& name);
        PAnimationState GetAnimationState(PAnimation animation);
        typedef std::map<std::string, PAnimationControl> Animations;
        Animations animations_;
        typedef std::vector<PAnimationState> AnimationStates;
        AnimationStates animationStates_;
        PSceneNode sceneNode_;
        SignalUpdate::PSlot slotUpdate_;
    };
}