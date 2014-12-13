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
#include "AnimationConverter.h"
#include "UtilConverter.h"
#include "Animation.h"
#include "Scene.h"
#include "Log.h"
#include "assimp/anim.h"
#include "assimp/scene.h"
#include <vector>

namespace NSG
{
	AnimationConverter::AnimationConverter(PScene scene, const aiAnimation* anim, PAnimation outAnim)
		: defaultTicksPerSecond_(4800.0f),
		scene_(scene)
	{
		float duration = (float)anim->mDuration;

		float ticksPerSecond = (float)anim->mTicksPerSecond;
		// If ticks per second not specified, it's probably a .X file. In this case use the default tick rate
		if (ticksPerSecond < glm::epsilon<float>())
			ticksPerSecond = defaultTicksPerSecond_;
		float tickConversion = 1.0f / ticksPerSecond;

		// Find out the start time of animation from each channel's first keyframe for adjusting the keyframe times
		// to start from zero
		float startTime = duration;
		for (unsigned j = 0; j < anim->mNumChannels; ++j)
		{
			aiNodeAnim* channel = anim->mChannels[j];
			if (channel->mNumPositionKeys > 0)
				startTime = std::min<float>(startTime, (float)channel->mPositionKeys[0].mTime);
			if (channel->mNumRotationKeys > 0)
				startTime = std::min<float>(startTime, (float)channel->mRotationKeys[0].mTime);
			if (channel->mScalingKeys > 0)
				startTime = std::min<float>(startTime, (float)channel->mScalingKeys[0].mTime);
		}

		duration -= startTime;

		outAnim->SetLength(duration * tickConversion);

		TRACE_LOG("Creating animation " << outAnim->GetName() << " length " << outAnim->GetLength());

		std::vector<AnimationTrack> tracks;
		for (unsigned j = 0; j < anim->mNumChannels; ++j)
		{
			aiNodeAnim* channel = anim->mChannels[j];
			std::string channelName = channel->mNodeName.C_Str();
			AnimationTrack track;
			if (scene_->GetName() == channelName)
				track.node_ = scene_;
			else
				track.node_ = scene_->GetChild<Node>(channelName, true);
			if (!track.node_.lock())
			{
				TRACE_LOG("Warning: skipping animation track " << channelName << " whose scene node was not found");
				return;
			}

			// Check which channels are used
			track.channelMask_ = 0;
			if (channel->mNumPositionKeys > 1)
				track.channelMask_ |= (int)AnimationChannel::POSITION;
			if (channel->mNumRotationKeys > 1)
				track.channelMask_ |= (int)AnimationChannel::ROTATION;
			if (channel->mNumScalingKeys > 1)
				track.channelMask_ |= (int)AnimationChannel::SCALE;
			// Check for redundant identity scale in all keyframes and remove in that case
			if (track.channelMask_ & (int)AnimationChannel::SCALE)
			{
				bool redundantScale = true;
				for (unsigned k = 0; k < channel->mNumScalingKeys; ++k)
				{
					float SCALE_EPSILON = 0.000001f;
					aiVector3t<float> scale = channel->mScalingKeys[k].mValue;
					Vector3 scaleVec(scale.x, scale.y, scale.z);
					if (fabsf(scaleVec.x - 1.0f) >= SCALE_EPSILON || fabsf(scaleVec.y - 1.0f) >= SCALE_EPSILON ||
						fabsf(scaleVec.z - 1.0f) >= SCALE_EPSILON)
					{
						redundantScale = false;
						break;
					}
				}
				if (redundantScale)
					track.channelMask_ &= ~(int)AnimationChannel::SCALE;
			}

			if (!track.channelMask_)
			{
				TRACE_LOG("Warning: skipping animation track " << channelName << " with no keyframes");
			}


			if ((channel->mNumPositionKeys > 1 && channel->mNumRotationKeys > 1 && channel->mNumPositionKeys != channel->mNumRotationKeys) ||
				(channel->mNumPositionKeys > 1 && channel->mNumScalingKeys > 1 && channel->mNumPositionKeys != channel->mNumScalingKeys) ||
				(channel->mNumRotationKeys > 1 && channel->mNumScalingKeys > 1 && channel->mNumRotationKeys != channel->mNumScalingKeys))
			{
				TRACE_LOG("Warning: differing amounts of channel keyframes, skipping animation track " << channelName);
				return;
			}

			unsigned keyFrames = channel->mNumPositionKeys;
			if (channel->mNumRotationKeys > keyFrames)
				keyFrames = channel->mNumRotationKeys;
			if (channel->mNumScalingKeys > keyFrames)
				keyFrames = channel->mNumScalingKeys;

			for (unsigned k = 0; k < keyFrames; ++k)
			{
				AnimationKeyFrame kf;
				kf.time_ = 0.0f;
				kf.position_ = VECTOR3_ZERO;
				kf.rotation_ = QUATERNION_IDENTITY;
				kf.scale_ = VECTOR3_ONE;

				// Get time for the keyframe. Adjust with animation's start time
				if (track.channelMask_ & (int)AnimationChannel::POSITION && k < channel->mNumPositionKeys)
					kf.time_ = ((float)channel->mPositionKeys[k].mTime - startTime) * tickConversion;
				else if (track.channelMask_ & (int)AnimationChannel::ROTATION && k < channel->mNumRotationKeys)
					kf.time_ = ((float)channel->mRotationKeys[k].mTime - startTime) * tickConversion;
				else if (track.channelMask_ & (int)AnimationChannel::SCALE && k < channel->mNumScalingKeys)
					kf.time_ = ((float)channel->mScalingKeys[k].mTime - startTime) * tickConversion;

				// Make sure time stays positive
				kf.time_ = std::max<float>(kf.time_, 0.0f);

				// Start with the bone's base transform
				PNode node = track.node_.lock();
				Vector3 pos = node->GetPosition();
				Vector3 scale = node->GetScale();
				Quaternion rot = node->GetOrientation();
				// Then apply the active channels
				if (track.channelMask_ & (int)AnimationChannel::POSITION && k < channel->mNumPositionKeys)
					pos = ToVector3(channel->mPositionKeys[k].mValue);
				if (track.channelMask_ & (int)AnimationChannel::ROTATION && k < channel->mNumRotationKeys)
					rot = ToQuaternion(channel->mRotationKeys[k].mValue);
				if (track.channelMask_ & (int)AnimationChannel::SCALE && k < channel->mNumScalingKeys)
					scale = ToVector3(channel->mScalingKeys[k].mValue);

#if 0
				// If root bone, transform with the model root node transform
				if (model && isRootBone)
				{
					aiMatrix4x4 transMat, scaleMat, rotMat;
					aiMatrix4x4::Translation(pos, transMat);
					aiMatrix4x4::Scaling(scale, scaleMat);
					rotMat = aiMatrix4x4(rot.GetMatrix());
					aiMatrix4x4 tform = transMat * rotMat * scaleMat;
					tform = GetDerivedTransform(tform, boneNode, model->rootNode_);
					tform.Decompose(scale, rot, pos);
				}
#endif

				if (track.channelMask_ & (int)AnimationChannel::POSITION)
					kf.position_ = pos;
				if (track.channelMask_ & (int)AnimationChannel::ROTATION)
					kf.rotation_ = rot;
				if (track.channelMask_ & (int)AnimationChannel::SCALE)
					kf.scale_ = scale;

				track.keyFrames_.push_back(kf);
			}

			tracks.push_back(track);
		}

		outAnim->SetTracks(tracks);
	}

	AnimationConverter::~AnimationConverter()
	{

	}
}
