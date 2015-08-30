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
#include "Animation.h"
#include "Node.h"
#include "Bone.h"
#include "Scene.h"
#include "Constants.h"
#include "Util.h"
#include "pugixml.hpp"
#include <algorithm>
#include <sstream>

namespace NSG
{
	template<> std::map<std::string, PAnimation> StrongFactory<std::string, Animation>::objsMap_ = std::map<std::string, PAnimation>{};

    AnimationKeyFrame::AnimationKeyFrame()
        : time_(0),
          scale_(VECTOR3_ONE),
          mask_((int)AnimationChannel::NONE)
    {
    }

    AnimationKeyFrame::AnimationKeyFrame(float time, Node* node)
        : time_(time),
          position_(node->GetPosition()),
          rotation_(node->GetOrientation()),
          scale_(node->GetScale()),
          mask_((int)AnimationChannel::ALL)
    {
    }

    void AnimationKeyFrame::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("KeyFrame");

        child.append_attribute("time").set_value(time_);
        if(position_ != VECTOR3_ZERO)
            child.append_attribute("position").set_value(ToString(position_).c_str());
        if(rotation_ != QUATERNION_IDENTITY)
            child.append_attribute("rotation").set_value(ToString(rotation_).c_str());
        if(scale_ != VECTOR3_ONE)
            child.append_attribute("scale").set_value(ToString(scale_).c_str());
    }

    void AnimationKeyFrame::Load(const pugi::xml_node& node)
    {
        time_ = node.attribute("time").as_float();
        position_ = VECTOR3_ZERO;
        auto posAtt = node.attribute("position");
        if(posAtt)
        {
            position_ = ToVertex3(posAtt.as_string());
            mask_ |= (int)AnimationChannel::POSITION;
        }
        rotation_ = QUATERNION_IDENTITY;
        auto rotAtt = node.attribute("rotation");
        if(rotAtt)
        {
            rotation_ = ToQuaternion(rotAtt.as_string());
            mask_ |= (int)AnimationChannel::ROTATION;
        }
        scale_ = VECTOR3_ONE;
        auto scaAtt = node.attribute("scale");
        if(scaAtt)
        {
            scale_ = ToVertex3(scaAtt.as_string());
            mask_ |= (int)AnimationChannel::SCALE;
        }
    }

    void AnimationKeyFrame::SetPose(PBone bone)
    {
        Matrix4 m = bone->GetPose() * ComposeMatrix(position_, rotation_, scale_);
        DecomposeMatrix(m, position_, rotation_, scale_);
    }

    void AnimationTrack::GetKeyFrameIndex(float time, size_t& index) const
    {
        if (time < 0)
            time = 0;

        if (index >= keyFrames_.size())
            index = keyFrames_.size() - 1;

        // Check for being too far ahead
        while (index && time < keyFrames_[index].time_)
            --index;

        // Check for being too far behind
        while (index < keyFrames_.size() - 1 && time >= keyFrames_[index + 1].time_)
            ++index;
    }

    void AnimationTrack::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Track");
        child.append_attribute("nodeName") = nodeName_.c_str();
        child.append_attribute("channelMask") = channelMask_.to_string().c_str();
        if (keyFrames_.size())
        {
            pugi::xml_node childFrames = child.append_child("KeyFrames");
            for (auto& obj : keyFrames_)
                obj.Save(childFrames);
        }
    }

    void AnimationTrack::Load(const pugi::xml_node& node)
    {
        nodeName_ = node.attribute("nodeName").as_string();
        std::string mask = node.attribute("channelMask").as_string();
        channelMask_ = AnimationChannelMask(mask);
        pugi::xml_node childKeyFrames = node.child("KeyFrames");
        if (childKeyFrames)
        {
            pugi::xml_node child = childKeyFrames.child("KeyFrame");
            while (child)
            {
                AnimationKeyFrame keyFrame;
                keyFrame.Load(child);
                keyFrames_.push_back(keyFrame);
                child = child.next_sibling("KeyFrame");
            }
            ResolveKeyFrameGaps();
        }
    }

    void AnimationTrack::ResolveKeyFrameGaps()
    {
        auto n = keyFrames_.size();
        if(n > 1)
        {
            for(int i=0; i<n; i++)
            {
                auto& current = keyFrames_[i];
                if(!(current.mask_ & (int)AnimationChannel::POSITION))
                    ResolvePositionGap(current.position_, i+1);
                if(!(current.mask_ & (int)AnimationChannel::ROTATION))
                    ResolveRotationGap(current.rotation_, i+1);
                if(!(current.mask_ & (int)AnimationChannel::SCALE))
                    ResolveScaleGap(current.scale_, i+1);
            }
        }
    }

    void AnimationTrack::ResolvePositionGap(Vector3& position, int frame)
    {
        auto n = keyFrames_.size();
        if(n > frame)
        {
            for(int i=frame; i<n; i++)
            {
                auto& current = keyFrames_[i];
                if(current.mask_ & (int)AnimationChannel::POSITION)
                {
                    position = current.position_;
                    break;
                }
            }
        }
    }

    void AnimationTrack::ResolveRotationGap(Quaternion& rotation, int frame)
    {
        auto n = keyFrames_.size();
        if(n > frame)
        {
            for(int i=frame; i<n; i++)
            {
                auto& current = keyFrames_[i];
                if(current.mask_ & (int)AnimationChannel::ROTATION)
                {
                    rotation = current.rotation_;
                    break;
                }
            }
        }
    }

    void AnimationTrack::ResolveScaleGap(Vector3& scale, int frame)
    {
        auto n = keyFrames_.size();
        if(n > frame)
        {
            for(int i=frame; i<n; i++)
            {
                auto& current = keyFrames_[i];
                if(current.mask_ & (int)AnimationChannel::SCALE)
                {
                    scale = current.scale_;
                    break;
                }
            }
        }
    }

    void AnimationTrack::ResolveFor(PBone bone)
    {
        node_ = bone;
        for (auto& kf : keyFrames_)
            kf.SetPose(bone);
    }

    Animation::Animation(const std::string& name)
        : Object(name),
          length_(0)
    {

    }

    Animation::~Animation()
    {
    }

    PAnimation Animation::Clone() const
    {
        auto clone = std::make_shared<Animation>(name_);
        clone->length_ = length_;
        clone->tracks_ = tracks_;
        return clone;
    }

    void Animation::ResolveFor(PSceneNode node)
    {
        for (auto& track : tracks_)
        {
            auto bone = node->GetChild<Bone>(track.nodeName_, true);
			if (bone)
                track.ResolveFor(bone);
        }
    }

    void Animation::SetLength(float length)
    {
        length_ = std::max<float>(length, 0);
    }

    void Animation::SetTracks(const std::vector<AnimationTrack>& tracks)
    {
        tracks_ = tracks;
    }

    void Animation::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Animation");
        child.append_attribute("name").set_value(name_.c_str());
        child.append_attribute("length").set_value(length_);
        if (tracks_.size())
        {
            pugi::xml_node childTracks = child.append_child("Tracks");
            for (auto& obj : tracks_)
                obj.Save(childTracks);
        }
    }

    void Animation::Load(const pugi::xml_node& node)
    {
        tracks_.clear();
        length_ = node.attribute("length").as_float();

        pugi::xml_node childTracks = node.child("Tracks");
        if (childTracks)
        {
            pugi::xml_node child = childTracks.child("Track");
            while (child)
            {
                AnimationTrack track;
                track.Load(child);
                AddTrack(track);
                child = child.next_sibling("Track");
            }
        }
    }

    void Animation::AddTrack(const AnimationTrack& track)
    {
        tracks_.push_back(track);
    }

	void Animation::SaveAnimations(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Animations");
		auto animations = Animation::GetObjs();
		for (auto& obj : animations)
            obj->Save(child);
    }
}