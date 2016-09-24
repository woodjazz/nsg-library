/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "AnimationState.h"
#include "Bone.h"
#include "Util.h"
#include "Log.h"
#include "Maths.h"

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
          weight_(0)
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
        auto length = animation_->GetLength();

        if (delta == 0 || length == 0)
            return;

        auto time = timePosition_ + delta;

        if (looped_)
        {
            while (time >= length)
                time -= length;
            while (time < 0)
                time += length;
        }

        SetTime(time);
    }

    bool AnimationState::HasEnded() const
    {
        if (looped_)
            return false;
        return timePosition_ >= animation_->GetLength();
    }

    void AnimationState::SetTime(float time)
    {
        if (!animation_)
            return;
        timePosition_ = Clamp(time, 0.f, animation_->GetLength());
    }

    float AnimationState::GetLength() const
    {
        return animation_ ? animation_->GetLength() : 0;
    }

    void AnimationState::SetWeight(float weight)
    {
        weight_ = Clamp(weight, 0.f, 1.f);
    }

    void AnimationState::Update()
    {
        for (auto& track : tracks_)
        {
            if (Equals(weight_, 0))
                continue;
            Blend(track, weight_);
        }
    }

    void AnimationState::Blend(AnimationStateTrack& track, float weight)
    {
        size_t& frame = track.currentKeyFrame_;
        track.GetKeyFrameIndex(timePosition_, frame);
        const AnimationKeyFrame* keyFrame = &track.keyFrames_[frame];
        auto bone = track.node_.lock();
        if (!bone) return;
        size_t nextFrame = frame + 1;
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
        if (!interpolate)
        {
            // No interpolation, blend between old transform & animation
            if (track.channelMask_ & (int)AnimationChannel::POSITION)
                bone->SetPosition(bone->GetPosition().Lerp(keyFrame->position_, weight));
            if (track.channelMask_ & (int)AnimationChannel::ROTATION)
                bone->SetOrientation(bone->GetOrientation().Slerp(keyFrame->rotation_, weight));
            if (track.channelMask_ & (int)AnimationChannel::SCALE)
                bone->SetScale(bone->GetScale().Lerp(keyFrame->scale_, weight));
        }
        else
        {
            const AnimationKeyFrame& nextKeyFrame = track.keyFrames_[nextFrame];
            auto timeInterval = nextKeyFrame.time_ - keyFrame->time_;
            if (timeInterval < 0.0f)
                timeInterval += animation_->GetLength();
            auto t = timeInterval > 0 ? (timePosition_ - keyFrame->time_) / timeInterval : 1;
           
            // Interpolation, blend between old transform & animation
            if (track.channelMask_ & (int)AnimationChannel::POSITION)
                bone->SetPosition(bone->GetPosition().Lerp(keyFrame->position_.Lerp(nextKeyFrame.position_, t), weight));
            if (track.channelMask_ & (int)AnimationChannel::ROTATION)
                bone->SetOrientation(bone->GetOrientation().Slerp(keyFrame->rotation_.Slerp(nextKeyFrame.rotation_, t), weight));
            if (track.channelMask_ & (int)AnimationChannel::SCALE)
                bone->SetScale(bone->GetScale().Lerp(keyFrame->scale_.Lerp(nextKeyFrame.scale_, t), weight));
        }
    }

    void AnimationState::SetLooped(bool looped)
    {
        looped_ = looped;
    }
}