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
#ifndef USING_BLENDER_FILES
#include "Blender.h"
#else
namespace Blender
{
#include "DNA_ID.h"
#include "DNA_action_types.h"
#include "DNA_actuator_types.h"
#include "DNA_anim_types.h"
#include "DNA_armature_types.h"
#include "DNA_boid_types.h"
#include "DNA_brush_types.h"
#include "DNA_camera_types.h"
#include "DNA_cloth_types.h"
#include "DNA_color_types.h"
#include "DNA_constraint_types.h"
#include "DNA_controller_types.h"
#include "DNA_curve_types.h"
#include "DNA_customdata_types.h"
#include "DNA_defs.h"
#include "DNA_documentation.h"
#include "DNA_dynamicpaint_types.h"
#include "DNA_effect_types.h"
#include "DNA_fileglobal_types.h"
#include "DNA_freestyle_types.h"
#include "DNA_genfile.h"
#include "DNA_gpencil_types.h"
#include "DNA_gpu_types.h"
#include "DNA_group_types.h"
#include "DNA_image_types.h"
//#include "DNA_ipo_types.h"
#include "DNA_key_types.h"
#include "DNA_lamp_types.h"
#include "DNA_lattice_types.h"
#include "DNA_linestyle_types.h"
#include "DNA_listBase.h"
#include "DNA_material_types.h"
#include "DNA_mesh_types.h"
#include "DNA_meshdata_types.h"
#include "DNA_meta_types.h"
#include "DNA_modifier_types.h"
#include "DNA_nla_types.h"
#include "DNA_node_types.h"
#include "DNA_object_fluidsim.h"
#include "DNA_object_force.h"
#include "DNA_object_types.h"
#include "DNA_outliner_types.h"
#include "DNA_packedFile_types.h"
#include "DNA_particle_types.h"
#include "DNA_property_types.h"
#include "DNA_rigidbody_types.h"
#include "DNA_scene_types.h"
#include "DNA_screen_types.h"
#include "DNA_sdna_types.h"
#include "DNA_sensor_types.h"
#include "DNA_sequence_types.h"
#include "DNA_smoke_types.h"
#include "DNA_sound_types.h"
#include "DNA_space_types.h"
#include "DNA_speaker_types.h"
#include "DNA_text_types.h"
#include "DNA_texture_types.h"
#include "DNA_userdef_types.h"
#include "DNA_vec_types.h"
#include "DNA_vfont_types.h"
#include "DNA_view2d_types.h"
#include "DNA_view3d_types.h"
#include "DNA_windowmanager_types.h"
#include "DNA_world_types.h"
#include "DNA_movieclip_types.h"
#include "DNA_tracking_types.h"
#include "DNA_mask_types.h"
};
#endif
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
		NSG::PSceneNode CreateSkeletonBones(const Blender::Object* objArmature, NSG::PSceneNode parent, const Blender::Scene* bscene);
		const Blender::Object* GetMesh(const Blender::Object* objArmature, const Blender::Scene* bscene) const;
		void BuildBoneTree(const Blender::Bone* cur, NSG::PNode parent, NSG::PSkeleton skeleton);
		void ConvertGroups(bParse::bMain* data, NSG::PScene scene, const Blender::Scene* bscene);
		void ConvertGroup(Blender::Group* bgroup, NSG::PScene scene, const Blender::Scene* bscene);
		void ConvertGroupInstances(NSG::PScene scene, const Blender::Scene* bscene);
		NSG::PSceneNode CreateCamera(const Blender::Object* obj, NSG::PSceneNode parent, const Blender::Scene* bscene);
        NSG::PSceneNode CreateLight(const Blender::Object* obj, NSG::PSceneNode parent);
		NSG::PSceneNode CreateMesh(const Blender::Object* obj, NSG::PSceneNode parent);
		bool ConvertMeshLegacy(const Blender::Object* obj, const Blender::Mesh* me, NSG::PModelMesh mesh, int materialIndex);
		bool ConvertMesh(const Blender::Object* obj, const Blender::Mesh* me, NSG::PModelMesh mesh, int materialIndex);
		int GetUVLayersBMmesh(const Blender::Mesh* me, Blender::MLoopUV** uvEightLayerArray, char** uvNames);
		int GetUVLayersBMmeshLegacy(const Blender::Mesh* mesh, Blender::MTFace** eightLayerArray, char** uvNames, Blender::MCol** oneMCol);
		void AssignBonesAndWeights(const Blender::Object* obj, const Blender::Mesh* me, NSG::VertexsData& vertexes);
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
		NSG::PScene LoadScene(const Blender::Scene* bscene);
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
		void CreateOffsetMatrices(const Blender::Object* obAr, const Blender::Object* objMesh, NSG::PSceneNode armatureNode, NSG::PSkeleton skeleton);
		const Blender::Object* GetAssignedArmature(const Blender::Object *obj) const;
		void GetBoneIndexByDeformGroupIndex(const Blender::Object* obj, const Blender::Object* obAr, std::vector<std::pair<int, std::string>>& list);
		void GetFrames(const Blender::bAction* action, std::vector<float> &fra);
		NSG::PScene CreateScene(const Blender::Scene* bscene);
		void LoadScenes(bParse::bMain* data);
		void ConvertGroupInstances(const std::string& groupName, NSG::PSceneNode parent);
		bool IsKnownType(const Blender::Object* obj) const;
		void Tessellate(NSG::PModelMesh mesh, const NSG::VertexsData& vertexData, const std::vector<int> & indexes);
		NSG::PhysicsShape GetShapeType(const Blender::Object* obj) const;
		NSG::PhysicsBody GetBodyType(char bodyType) const;
		bool IsRigidBody(const Blender::Object* obj) const;
		bool HasCollisionShape(const Blender::Object* obj) const;
		bool HasPhysics(const Blender::Object* obj) const;
		bool ShapeNeedsMesh(NSG::PhysicsShape shapeType) const;
		NSG::PSceneNode GetParentWithRigidBody(NSG::PSceneNode sceneNode) const;
		NSG::PRigidBody CreateRigidBody(NSG::PSceneNode sceneNode, const Blender::Object* obj) const;
    private:
        NSG::Path path_;
        NSG::Path outputDir_;
		std::vector<const Blender::Object*> physics_;
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