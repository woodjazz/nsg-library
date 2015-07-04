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
	class BGroup;
    class BScene
    {
    public:
		BScene(const NSG::Path& path, const NSG::Path& outputDir, bool embedResources);
        ~BScene();
        bool Load();
		bool Save(bool compress);
		void GenerateXML(pugi::xml_document& doc);
    private:
        void ExtractGeneral(const Blender::Object* obj, NSG::PSceneNode sceneNode);
		NSG::PSceneNode ConvertObject(const Blender::Object* obj, NSG::PScene scene, const Blender::Scene* bscene);
		NSG::PSceneNode CreateSceneNode(const Blender::Object* obj, NSG::PSceneNode parent);
		NSG::PSceneNode CreateSkeletonBones(const Blender::Object* obj, NSG::PSceneNode parent);
		void BuildBoneTree(const std::string& armatureName, const Blender::Bone* cur, NSG::PSceneNode parent);
		void ConvertGroups(bParse::bMain* data, NSG::PScene scene, const Blender::Scene* bscene);
		void ConvertGroupInstances(bParse::bMain* data, NSG::PScene scene, const Blender::Scene* bscene);
		NSG::PSceneNode CreateCamera(const Blender::Object* obj, NSG::PSceneNode parent, const Blender::Scene* bscene);
        NSG::PSceneNode CreateLight(const Blender::Object* obj, NSG::PSceneNode parent);
		NSG::PSceneNode CreateMesh(const Blender::Object* obj, NSG::PSceneNode parent);
		bool ConvertMeshLegacy(const Blender::Object* obj, const Blender::Mesh* me, NSG::PModelMesh mesh, int materialIndex);
		bool ConvertMesh(const Blender::Object* obj, const Blender::Mesh* me, NSG::PModelMesh mesh, int materialIndex);
		int GetUVLayersBMmesh(const Blender::Mesh* me, Blender::MLoopUV** uvEightLayerArray, char** uvNames);
		int GetUVLayersBMmeshLegacy(const Blender::Mesh* mesh, Blender::MTFace** eightLayerArray, char** uvNames, Blender::MCol** oneMCol);
		void AssignBonesAndWeights(const Blender::Object* obj, const Blender::Mesh* me, NSG::VertexsData& vertexes);
		void CreateSkeleton(NSG::PScene scene, const Blender::Object* obj);
		std::vector<NSG::PSound> LoadSounds(bParse::bMain* data);
		NSG::PSound LoadSound(Blender::bSound* sound);
		const Blender::Material* GetMaterial(const Blender::Object* ob, int index) const;
		std::vector<NSG::PMaterial> LoadMaterials(bParse::bMain* data);
		NSG::PMaterial LoadMaterial(const Blender::Material* mt);
		NSG::PTexture CreateTexture(const Blender::Image* ima, const std::string& imageName);
		void SetMaterial(const Blender::Object* obj, NSG::PSceneNode sceneNode, int materialIndex);
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
		void LoadPhysics(NSG::PScene scene, const Blender::Object* obj);
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
		void ConvertGroupInstances(const std::string& groupName, NSG::PSceneNode parent);
		bool IsKnownType(const Blender::Object* obj) const;
		void Tessellate(NSG::PModelMesh mesh, const NSG::VertexsData& vertexData, const std::vector<int> & indexes);
		NSG::PhysicsShape GetShapeType(short boundtype) const;
		NSG::PhysicsBody GetBodyType(char bodyType) const;
		bool IsRigidBody(NSG::PhysicsBody bodyType) const;
		bool ShapeNeedsMesh(NSG::PhysicsShape shapeType) const;
		NSG::PSceneNode GetParentWithRigidBody(NSG::PSceneNode sceneNode) const;
    private:
        NSG::Path path_;
        NSG::Path outputDir_;
		std::vector<const Blender::Object*> armatureLinker_;
		std::vector<const Blender::Object*> physics_;
		std::map<std::string, std::vector<NSG::PWeakNode>> armatureBones_;
		bool embedResources_;
		std::vector<NSG::PScene> scenes_;
		std::vector<const Blender::Scene*> bscenes_;
		std::vector<NSG::PSound> sounds_;
        NSG::PMaterial defaultMaterial_;
        typedef std::shared_ptr<BGroup> PBGroup;
        typedef std::map<std::string, PBGroup> Groups;
        Groups groups_;
    };
}