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
#include "assimp/IOSystem.hpp"
#include <string>
#include <vector>
#include <map>

struct aiScene;
struct aiMesh;
struct aiNode;
struct aiLight;
struct aiCamera;

namespace pugi
{
    class xml_node;
}


namespace NSG
{
    class SceneConverter : public Assimp::IOSystem
    {
    public:
        SceneConverter(const Path& path);
        ~SceneConverter();
        bool Exists(const char* filename) const override;
        char getOsSeparator() const override;
        Assimp::IOStream* Open(const char* filename, const char* mode = "rb") override;
        void Close(Assimp::IOStream* pFile) override;
        bool Save(const std::string& filename);
        bool Load();
		PScene GetScene() { return scene_; }
    private:
        Matrix4 GetOffsetMatrix(const aiMesh* mesh, const aiNode* rootNode, const aiNode* node, const std::string& boneName);
        void GetFinal(std::vector<aiNode*>& dest, const std::set<aiNode*>& necessary, aiNode* node);
        const aiNode* GetMeshNode(const aiScene* sc, const aiNode* node, const aiMesh* aiMesh);
        void MarkProgramAsSkinableNodes(const Mesh* mesh);
        void GetBlendData(PMesh mesh, const aiMesh* aiMesh) const;
        void MakeSkeleton(const aiMesh* aiMesh, PModelMesh mesh, const aiNode* rootBone, const std::vector<aiNode*>& bones);
        aiNode* GetNode(const std::string& name, aiNode* rootNode);
        void LoadBones(const aiScene* sc, CachedData& data);
        void LoadBones(const aiScene* sc, const aiMesh* aiMesh, PModelMesh mesh);
        void Load(const aiScene* sc);
        void LoadAnimations(const aiScene* sc);
        const aiCamera* GetCamera(const aiScene* sc, const aiString& name) const;
        const aiLight* GetLight(const aiScene* sc, const aiString& name) const;
        void LoadMeshesAndMaterials(const aiScene* sc, CachedData& data);
        void RecursiveLoad(const aiScene* sc, const aiNode* nd, SceneNode* sceneNode, const CachedData& data);
        PScene scene_;
        Path path_;
    };
}