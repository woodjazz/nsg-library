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
        BScene(const NSG::Path& path, const NSG::Path& outputDir);
        ~BScene();
        bool Load();
        bool Save();
    private:
        void ExtractGeneral(const Blender::Object* obj, NSG::PSceneNode sceneNode);
		void ConvertObject(const Blender::Object* obj, NSG::PSceneNode sceneNode);
		NSG::PSceneNode CreateSceneNode(const Blender::Object* obj, NSG::PSceneNode parent);
		void CreateSkeleton(const Blender::Object* obj, NSG::PSceneNode parent);
		void BuildBoneTree(const Blender::Bone* cur, const Blender::Bone* prev, NSG::PSceneNode parent);
        void CreateCamera(const Blender::Object* obj, NSG::PSceneNode parent);
        void CreateLight(const Blender::Object* obj, NSG::PSceneNode parent);
        void CreateMesh(const Blender::Object* obj, NSG::PSceneNode parent);
        void ConvertMesh(Blender::Mesh* me, NSG::PModelMesh mesh);
		//void CalcNormal(TempFace* tri);
		void GetLayersBMmesh(Blender::Mesh* me, Blender::MTexPoly** eightLayerArray, Blender::MLoopUV** uvEightLayerArray, Blender::MLoopCol** oneMCol, int& validLayers);
		void LoadBones(const Blender::Object* obj, const Blender::Mesh* me, NSG::PModelMesh mesh);
		void LoadSkeleton(const Blender::Object* obj);
		void MakeBonesList(const Blender::Bone* bone, std::vector<NSG::PWeakNode>& nodeBones);
		void LoadMaterials(const Blender::Object* obj);
		const Blender::Material* GetMaterial(const Blender::Object* ob, int index) const;
		void LoadMaterials(bParse::bMain* data);
		void LoadMaterial(const Blender::Material* mt);
		NSG::PTexture CreateTexture(const std::string& fileName);
		void SetMaterial(const Blender::Object* obj, NSG::PSceneNode sceneNode);
		void LoadAnimData(NSG::PSceneNode sceneNode, const Blender::AnimData* adt, float animfps);
		float ConvertAction(const Blender::bAction* action, float animfps);
		void GetActionStartEnd(const Blender::bAction* action, float& start, float& end);
		void GetSplineStartEnd(const Blender::BezTriple* bez, int totvert, float& start, float& end);
		PBSpline ConvertSpline(const Blender::BezTriple* bez, int access, int mode, int totvert, float xoffset, float xfactor, float yoffset, float yfactor);
    private:
        const Blender::Scene* sc_;
        NSG::Path path_;
        NSG::Path outputDir_;
        NSG::PScene scene_;
		std::vector<const Blender::Object*> armatureLinker_;
    };
}