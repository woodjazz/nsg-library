/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

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
#include "AnimationState.h"
#include "Node.h"
#include "Util.h"

namespace NSG
{
    AnimationStateTrack::AnimationStateTrack(const AnimationTrack& base)
        : currentKeyFrame_(0)
    {
        *static_cast<AnimationTrack*>(this) = base;
    }

    AnimationState::AnimationState(PAnimation animation)
        : animation_(animation),
          timePosition_(0),
          looped_(false),
		  speed_(1)
    {
        auto tracks = animation_->GetTracks();
        for (auto& track : tracks)
            tracks_.push_back(track);
    }

    AnimationState::~AnimationState()
    {
    }

    void AnimationState::AddTime(float delta)
    {
        if (!animation_ || !speed_)
            return;

        delta *= speed_;

        float length = animation_->GetLength();

        if (delta == 0.0f || length == 0.0f)
            return;

        float time = timePosition_ + delta;

        if (looped_)
        {
            while (time >= length)
                time -= length;
            while (time < 0.0f)
                time += length;
        }

        SetTime(time);
    }

    void AnimationState::SetTime(float time)
    {
        if (!animation_)
            return;

		timePosition_ = glm::clamp(time, 0.0f, animation_->GetLength());
    }


    void AnimationState::Update()
    {
        for (auto& track : tracks_)
        {
            unsigned& frame = track.currentKeyFrame_;
            track.GetKeyFrameIndex(timePosition_, frame);

            unsigned nextFrame = frame + 1;
            bool interpolate = true;
            if (nextFrame >= track.keyFrames_.size())
            {
                if (!looped_)
                {
                    nextFrame = frame;
                    interpolate = false;
                }
                else
                    nextFrame = 0;
            }

            const AnimationKeyFrame* keyFrame = &track.keyFrames_[frame];

            if (!interpolate)
            {
                // No interpolation, full weight
                if (track.channelMask_ & (int)AnimationChannel::POSITION)
                    track.node_->SetPosition(keyFrame->position_);
                if (track.channelMask_ & (int)AnimationChannel::ROTATION)
                    track.node_->SetOrientation(keyFrame->rotation_);
                if (track.channelMask_ & (int)AnimationChannel::SCALE)
                    track.node_->SetScale(keyFrame->scale_);
            }
            else
            {
                const AnimationKeyFrame& nextKeyFrame = track.keyFrames_[nextFrame];
                float timeInterval = nextKeyFrame.time_ - keyFrame->time_;
                if (timeInterval < 0.0f)
                    timeInterval += animation_->GetLength();
                float t = timeInterval > 0.0f ? (timePosition_ - keyFrame->time_) / timeInterval : 1.0f;

				// Interpolation, full weight
                if (track.channelMask_ & (int)AnimationChannel::POSITION)
					track.node_->SetPosition(Lerp(keyFrame->position_, nextKeyFrame.position_, t));
                if (track.channelMask_ & (int)AnimationChannel::ROTATION)
					track.node_->SetOrientation(glm::slerp(keyFrame->rotation_, nextKeyFrame.rotation_, t));
                if (track.channelMask_ & (int)AnimationChannel::SCALE)
					track.node_->SetScale(Lerp(keyFrame->scale_, nextKeyFrame.scale_, t));
            }
        }
    }

    void AnimationState::SetLooped(bool looped)
    {
    	looped_ = looped;
    }

    void AnimationState::SetSpeed(float speed)
    {
        speed_ = speed;
    }

}