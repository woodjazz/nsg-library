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
#include "Animation.h"
#include "Node.h"
#include "Scene.h"
#include "Constants.h"
#include "App.h"
#include "Util.h"
#include "pugixml.hpp"
#include <algorithm>
#include <sstream>

namespace NSG
{
    AnimationKeyFrame::AnimationKeyFrame()
        : time_(0),
          scale_(VECTOR3_ONE)
    {
    }

    AnimationKeyFrame::AnimationKeyFrame(float time, Node* node)
        : time_(time),
          position_(node->GetPosition()),
          rotation_(node->GetOrientation()),
          scale_(node->GetScale())
    {
    }

    void AnimationKeyFrame::Save(pugi::xml_node& node)
    {
		pugi::xml_node child = node.append_child("KeyFrame");

        {
            std::stringstream ss;
            ss << time_;
            child.append_attribute("time") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << position_;
            child.append_attribute("position") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << rotation_;
            child.append_attribute("rotation") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << scale_;
            child.append_attribute("scale") = ss.str().c_str();
        }
    }

	void AnimationKeyFrame::Load(const pugi::xml_node& node)
	{
		time_ = node.attribute("time").as_float();
		position_ = GetVertex3(node.attribute("position").as_string());
		rotation_ = GetQuaternion(node.attribute("rotation").as_string());
		scale_ = GetVertex3(node.attribute("scale").as_string());
	}

    void AnimationTrack::GetKeyFrameIndex(float time, unsigned& index) const
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
        child.append_attribute("nodeName") = node_.lock()->GetName().c_str();
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
		std::string nodeName = node.attribute("nodeName").as_string();
		PScene scene = scene_.lock();
		if (scene->GetName() == nodeName)
			node_ = scene;
		else
			node_ = scene->GetChild<Node>(nodeName, true);
        CHECK_ASSERT(node_.lock(), __FILE__, __LINE__);
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
		}

	}

	Animation::Animation(const std::string& name)
        : name_(name),
		length_(0)
    {

    }

    Animation::~Animation()
    {

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
        child.append_attribute("name") = name_.c_str();

        {
            std::stringstream ss;
            ss << length_;
            child.append_attribute("length") = ss.str().c_str();
        }

        if (tracks_.size())
        {
			pugi::xml_node childTracks = child.append_child("Tracks");
            for (auto& obj : tracks_)
				obj.Save(childTracks);
        }
    }

	void Animation::Load(const pugi::xml_node& node)
	{
		length_ = node.attribute("length").as_float();

		pugi::xml_node childTracks = node.child("Tracks");
		if (childTracks)
		{
			pugi::xml_node child = childTracks.child("Track");
			while (child)
			{
				AnimationTrack track;
				track.scene_ = scene_;
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

	void Animation::SetScene(PWeakScene scene)
	{
		scene_ = scene;
	}
}