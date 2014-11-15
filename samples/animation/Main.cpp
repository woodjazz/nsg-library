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

#include "NSG.h"
using namespace NSG;

struct Sample : App
{
    PScene scene_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = true;
    }

	void Start(int argc, char* argv[]) override
    {
		scene_ = GetCurrentScene();
		PResourceFile resource(GetOrCreateResourceFile("data/scene.xml"));
		scene_->Load(resource);
    }

	void OnSceneLoaded() override
	{
		PSceneNode objNode = scene_->GetOrCreateChild<SceneNode>("LOD3sp");
		Vertex3 objPos = objNode->GetGlobalPosition();
		BoundingBox objBB = objNode->GetWorldBoundingBox();
		objBB.max_ *= 1.75f;
		objBB.min_ *= 1.75f;
		PCamera camera = scene_->GetOrCreateChild<Camera>("camera1");
		camera->Activate();
		camera->SetGlobalPosition(Vector3(0, objBB.max_.y, objBB.max_.z));
		camera->SetGlobalLookAt(objPos);

		PAnimation animation = scene_->GetOrCreateAnimation("anim0");
		AnimationTrack track;
		track.node_ = camera;
		track.channelMask_ = (int)AnimationChannel::POSITION | (int)AnimationChannel::ROTATION;

		{
			AnimationKeyFrame key(0, camera.get());
			track.keyFrames_.push_back(key);
		}

		{
			PNode node = std::make_shared<Node>();
			node->SetParent(camera->GetParent());
			node->SetGlobalPosition(Vector3(objBB.max_.x, objBB.max_.y, 0));
			node->SetGlobalLookAt(objPos);
			AnimationKeyFrame key(2, node.get());
			track.keyFrames_.push_back(key);
		}

		{
			PNode node = std::make_shared<Node>();
			node->SetParent(camera->GetParent());
			node->SetGlobalPosition(Vector3(0, objBB.max_.y, objBB.min_.z));
			node->SetGlobalLookAt(objPos);
			AnimationKeyFrame key(4, node.get());
			track.keyFrames_.push_back(key);
		}

		{
			PNode node = std::make_shared<Node>();
			node->SetParent(camera->GetParent());
			node->SetGlobalPosition(Vector3(objBB.min_.x, objBB.max_.y, 0));
			node->SetGlobalLookAt(objPos);
			AnimationKeyFrame key(6, node.get());
			track.keyFrames_.push_back(key);
		}

		animation->AddTrack(track);
		animation->SetLength(8);

		scene_->PlayAnimation(animation, false);
	}
};

NSG_MAIN(Sample);
