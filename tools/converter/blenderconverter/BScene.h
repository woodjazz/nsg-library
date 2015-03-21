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
#include "Types.h"
#include "Path.h"
#include "VertexData.h"
#include "Blender.h"
#include "BSpline.h"
#include <vector>
#include <map>

namespace bParse 
{
	class bMain;
}

namespace BlenderConverter
{
    class BScene
    {
    public:
		BScene(const NSG::Path& path, const NSG::Path& outputDir, bool embedResources);
        ~BScene();
        bool Load();
		bool Save(bool compress);
    private:
        void ExtractGeneral(const Blender::Object* obj, NSG::PSceneNode sceneNode);
		void ConvertObject(const Blender::Object* obj, NSG::PSceneNode sceneNode);
		NSG::PSceneNode CreateSceneNode(const Blender::Object* obj, NSG::PSceneNode parent);
		void CreateSkeletonBones(const Blender::Object* obj, NSG::PSceneNode parent);
		void BuildBoneTree(const std::string& armatureName, const Blender::Bone* cur, NSG::PSceneNode parent);
        void CreateCamera(const Blender::Object* obj, NSG::PSceneNode parent);
        void CreateLight(const Blender::Object* obj, NSG::PSceneNode parent);
        void CreateMesh(const Blender::Object* obj, NSG::PSceneNode parent);
		void ConvertMesh(const Blender::Object* obj, const Blender::Mesh* me, NSG::PModelMesh mesh);
		int GetUVLayersBMmesh(const Blender::Mesh* me, Blender::MLoopUV** uvEightLayerArray);
		void AssignBonesAndWeights(const Blender::Object* obj, const Blender::Mesh* me, NSG::VertexsData& vertexes);
		void CreateSkeleton(NSG::PScene scene, const Blender::Object* obj);
		void LoadMaterials(const Blender::Object* obj);
		const Blender::Material* GetMaterial(const Blender::Object* ob, int index) const;
		void LoadMaterials(bParse::bMain* data);
		void LoadMaterial(const Blender::Material* mt);
		NSG::PTexture CreateTexture(const Blender::Image* ima);
		void SetMaterial(const Blender::Object* obj, NSG::PSceneNode sceneNode);
		void LoadAnimData(NSG::PSceneNode sceneNode, const Blender::AnimData* adt, float animfps);
		void CreateAnimations(bParse::bMain* data);
		void CreateAnimation(const Blender::bAction* action, const Blender::Scene* bscene, NSG::PScene scene);
		struct TrackData
		{
			NSG::PAnimationTrack track;
			std::vector<PBSpline> keyframes;
		};
		typedef std::shared_ptr<TrackData> PTrackData;
		typedef std::map<std::string, PTrackData> BTracks;
		float GetTracks(const Blender::bAction* action, float animfps, BTracks& tracks);
		void ConvertTracks(NSG::PScene scene, const Blender::bAction* action, NSG::PAnimation anim, BTracks& tracks, float length);
		void GetActionStartEnd(const Blender::bAction* action, float& start, float& end);
		void GetSplineStartEnd(const Blender::BezTriple* bez, int totvert, float& start, float& end);
		PBSpline ConvertSpline(const Blender::BezTriple* bez, SPLINE_CHANNEL_CODE access, int mode, int totvert, float xoffset, float xfactor, float yoffset, float yfactor);
		NSG::AnimationChannelMask ConvertChannel(const Blender::bAction* action, PTrackData trackData, NSG::AnimationTrack& track, float timeFrameLength);
		void LoadPhysics(const Blender::Object* obj, NSG::PSceneNode sceneNode);
		void MarkProgramAsSkinableNodes(const NSG::Mesh* mesh);
		const void* FindByString(const Blender::ListBase* listbase, const char* id, int offset) const;
		const Blender::bPoseChannel* GetPoseChannelByName(const Blender::bPose* pose, const char* name) const;
		const Blender::Bone* GetBoneFromDefGroup(const Blender::Object* obj, const Blender::bDeformGroup* def) const;
		bool IsBoneDefGroup(const Blender::Object* obj, const Blender::bDeformGroup* def) const;
		void CreateOffsetMatrices(const Blender::Object* obj, NSG::PSceneNode armatureNode);
		const Blender::Object* GetAssignedArmature(const Blender::Object *obj) const;
		void GetBoneIndexByDeformGroupIndex(const Blender::Object* obj, const Blender::Object* obAr, std::vector<std::pair<int, std::string>>& list);
		void GetFrames(const Blender::bAction* action, std::vector<float> &fra);
		NSG::PScene CreateScene(const Blender::Scene* bscene);
		void CreateScenes(bParse::bMain* data);
    private:
        NSG::Path path_;
        NSG::Path outputDir_;
		std::vector<const Blender::Object*> armatureLinker_;
		std::map<std::string, std::vector<NSG::PWeakNode>> armatureBones_;
		bool embedResources_;
		std::vector<NSG::PScene> scenes_;
		std::vector<const Blender::Scene*> bscenes_;
    };
}