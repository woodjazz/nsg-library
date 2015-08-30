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
#include "Object.h"
#include "StrongFactory.h"

namespace NSG
{
    struct AnimationKeyFrame
    {
        float time_;
        Vector3 position_;
        Quaternion rotation_;
        Vector3 scale_;
        AnimationChannelMask mask_;
        AnimationKeyFrame();
        AnimationKeyFrame(float time, Node* node);
        void Save(pugi::xml_node& node);
        void Load(const pugi::xml_node& node);
        void SetPose(PBone bone);
    };

    struct AnimationTrack
    {
        std::string nodeName_;
        PWeakNode node_; //not set till the animation is resolved (see Animation::ResolveFor)
        AnimationChannelMask channelMask_;
        std::vector<AnimationKeyFrame> keyFrames_;
        void GetKeyFrameIndex(float time, size_t& index) const;
        void Save(pugi::xml_node& node);
        void Load(const pugi::xml_node& node);
        void ResolveFor(PBone bone);
        void ResolveKeyFrameGaps();
        void ResolvePositionGap(Vector3& position, int frame);
        void ResolveRotationGap(Quaternion& rotation, int frame);
        void ResolveScaleGap(Vector3& scale, int frame);
    };

	class Animation : public Object, public std::enable_shared_from_this<Animation>, public StrongFactory<std::string, Animation>
    {
    public:
        Animation(const std::string& name);
        ~Animation();
        PAnimation Clone() const;
        void ResolveFor(PSceneNode node);
        const std::string& GetName() const { return name_; }
        void SetLength(float length);
        float GetLength() const { return length_; }
        void SetTracks(const std::vector<AnimationTrack>& tracks);
        const std::vector<AnimationTrack>& GetTracks() const { return tracks_; }
        void Save(pugi::xml_node& node);
        void Load(const pugi::xml_node& node) override;
        void AddTrack(const AnimationTrack& track);
        static void SaveAnimations(pugi::xml_node& node);
    private:
        float length_;
        std::vector<AnimationTrack> tracks_;
    };
}